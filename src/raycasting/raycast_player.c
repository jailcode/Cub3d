/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/02/10 12:01:31 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "raycast.h"
#include <stdbool.h>

bool	set_initial_player_pos(t_player *p, t_fidx init_player_field,
		t_cdir compassdir)
{
	if (p->pos.x < 0 || p->pos.y < 0)
		return (false);
	p->pos.x = (double)init_player_field.horizontal + 0.5;
	p->pos.y = (double)init_player_field.vertical + 0.5;
	p->dov = (t_dir){.x = 0.0, .y = 0.0};
	if (compassdir == (t_cdir)North)
		p->dov.y = -1.0;
	else if (compassdir == (t_cdir)East)
		p->dov.x = +1.0;
	else if (compassdir == (t_cdir)South)
		p->dov.y = +1.0;
	else if (compassdir == (t_cdir)West)
		p->dov.x = -1.0;
	else
		return (false);
	p->verticaldovrad = 0.0;
	return (true);
}

static bool	update_playpos_internal(t_game *const g, t_line ray,
		double const deltadist);

static bool	update_playpos_internal_slide(t_game *const g, t_line ray,
		t_rcintersect const *const pintersect, double const deltadist4slide)
{
	t_vec				wallparallel;
	double				slidedist;
	
	wallparallel = (t_vec){-pintersect->wallnormal.y, pintersect->wallnormal.x};
	slidedist = deltadist4slide * vdot(&ray.dir, &wallparallel);
	if (fabs(slidedist) > EPS)
	{
		ray.dir = wallparallel;
		if (slidedist < 0.0)
		{
			slidedist *= -1.0;
			ray.dir.x *= -1.0;
			ray.dir.y *= -1.0;
		}
		return (update_playpos_internal(g, ray, slidedist));
	}
	return (false);
}

static bool	update_playpos_internal(t_game *const g, t_line ray,
		double const deltadist)
{
	t_player			*const p = &g->player;
	t_rcintersect const intersect = rayintersection(ray, g->map);
	double const		maxdistbeforeimpact
		= intersect.dist2intersect - p->mindist2wall
		/ fabs(vdot(&ray.dir, &intersect.wallnormal));
	bool				player_moved;
	double				ratio;

	player_moved = false;
	ratio = maxdistbeforeimpact / deltadist;
	p->collision = (ratio < 1.0);
	if (ratio > EPS)
	{
		if (!p->collision)
			ratio = 1.0;
		p->pos.x += ray.dir.x * deltadist * ratio;
		p->pos.y += ray.dir.y * deltadist * ratio;
		player_moved = true;
	}
	if (p->collision)
	{
		player_moved = update_playpos_internal_slide(g, ray,
			&intersect, deltadist * (1.0 - ratio));
		// wallparallel = (t_vec){-intersect.wallnormal.y, intersect.wallnormal.x};
		// slidedist = deltadist * (1.0 - ratio) * vdot(&ray.dir, &wallparallel);
		// if (fabs(slidedist) > EPS)
		// {
		// 	ray.dir = wallparallel;
		// 	if (slidedist < 0.0)
		// 	{
		// 		slidedist *= -1.0;
		// 		ray.dir.x *= -1.0;
		// 		ray.dir.y *= -1.0;
		// 	}
		// 	player_moved = update_playpos_internal(g, ray, slidedist);
		// }
	}
	return (player_moved);
}

static bool	update_player_pos_shift(t_game *const g, t_vec deltapos)
{
	static t_vec const		maincsysxaxis = (t_vec){1.0, 0.0};
	double const			deltadist = vnorm(&deltapos);
	t_vec const				maincsyxaxis_rel2plyr
		= transform2csys(&maincsysxaxis, &g->player.dov);
	t_vec const				deltaposabs
		= transform2csys(&deltapos, &maincsyxaxis_rel2plyr);	
	t_line			ray;		

	g->player.collision = false;
	if (deltadist > EPS)
	{
		ray.origin = g->player.pos;
		ray.dir.x = deltaposabs.x / deltadist;
		ray.dir.y = deltaposabs.y / deltadist;
		return (update_playpos_internal(g, ray, deltadist));
	}
	return (false);
}

bool	update_player_pos(t_game *const g, t_vec deltapos,
		double const deltadovrad, double const deltaverticalrad)
{
	t_player *const	p = &g->player;
	bool			player_moved;
	double			pdovrad;

	player_moved = update_player_pos_shift(g, deltapos);
	if (fabs(deltadovrad) > EPS)
	{
		pdovrad = atan2(p->dov.y, p->dov.x) + deltadovrad;
		p->dov.x = cos(pdovrad);
		p->dov.y = sin(pdovrad);
		player_moved = true;
	}
	if (fabs(deltaverticalrad) > EPS)
	{
		p->verticaldovrad += deltaverticalrad;
		player_moved = true;
	}
	if (player_moved)
		return (generate_raycast(g), true);
	return (false);
}
