/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/01/29 18:13:14 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "../includes/cub.h"
#include "../includes/raycast.h"

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

t_coord	intersection(t_line const *const rayln, t_line const *const gridln)
{
	double const lambda = (
		  rayln->dir.x * (rayln->origin.y - gridln->origin.y)
		- rayln->dir.y * (rayln->origin.x - gridln->origin.x)
	) / (rayln->dir.x * gridln->dir.y - rayln->dir.y * gridln->dir.x);
	t_coord const intersect = (t_coord) { gridln->origin.x + lambda * gridln->dir.x, gridln->origin.y + lambda * gridln->dir.y };
	return (intersect);
}

double dist(t_coord const *const c1, t_coord const *const c2)
{
	return (hypot(c1->x - c2->x, c1->y -c2->y));
}

// double mindist(t_coord const *const c1, t_coord const *const c2)
// {
// 	return ((c1->x * c2->x) + (c1->y * c2->y));
// }
// double mindist(double const dist2intersect, double const deltadov)
// {
// 	return (dist2intersect * cos(deltadov));
// }

bool set_initial_player_pos(t_player *p, t_fidx init_player_field, t_cdir const compassdir)
{
	if (compassdir == (t_cdir)North)
		p->dov.rad = 1.5 * M_PI;
	else if (compassdir == (t_cdir)East)
		p->dov.rad = 0.0 * M_PI;
	else if (compassdir == (t_cdir)South)
		p->dov.rad = 0.5 * M_PI;
	else if (compassdir == (t_cdir)West)
		p->dov.rad = 1.0 * M_PI;
	else if (init_player_field.horizontal < 0 || init_player_field.vertical < 0)
		return (false);
	p->dov.x = cos(p->dov.rad);
	p->dov.y = sin(p->dov.rad);
	p->pos.x = (double)init_player_field.horizontal + 0.5;
	p->pos.y = (double)init_player_field.vertical + 0.5;
	return (true);
}

bool	is_wall(t_map const *const pmap, t_fidx const fidx)
{
	t_field const *const pfield = &pmap->main_map[fidx.horizontal][fidx.vertical];
	
	return (pfield->ftype == (t_fieldtype)wall);
}


t_rcintersect	rayintersection(t_line const ray, t_map const *const pmap)
{
	t_rcintersect rcintersect;
	bool	wall_found;
	t_gridlns	gridlines = {
		.horizontal = (t_line){/* .origin = (t_coord){0.0, 0.0}, */ .dir = (t_dir){.rad = M_PI / 2, .x = 1.0, .y = 0.0}},
		.vertical =  (t_line){/* .origin = (t_coord){0.0, 0.0}, */ .dir = (t_dir){.rad = M_PI / 2, .x = 0.0, .y = 1.0}},
	};
	
	gridlines.vertical.origin.x = (int)(ray.origin.x);
	if (ray.dir.x > 0)
		gridlines.vertical.origin.x += 1.0;
	gridlines.horizontal.origin.y = (int)(ray.origin.y);
	if (ray.dir.y > 0)
		gridlines.horizontal.origin.y += 1.0;
	
	wall_found = false;
	while (!wall_found)
	{
		t_coord const intersect_horizontalln = intersection(&ray, &gridlines.horizontal);
		t_coord const intersect_verticalln = intersection(&ray, &gridlines.vertical);		
		double const dist_h = dist(&ray.origin, &intersect_horizontalln);
		double const dist_v = dist(&ray.origin, &intersect_verticalln);
		bool hitverticalln;
		if (dist_v <= dist_h)
		{
			rcintersect.intersection = intersect_verticalln;
			rcintersect.dist2intersect = dist_v;				
			hitverticalln = true;
		}
		else
		{
			rcintersect.intersection = intersect_horizontalln;
			rcintersect.dist2intersect = dist_h;
			hitverticalln = false;
		}
		
		t_fidx fieldidx;
		if (hitverticalln)
			fieldidx = (t_fidx){
				.horizontal = (int)(round(rcintersect.intersection.x)),
				.vertical = (int)(rcintersect.intersection.y)};
		else
			fieldidx = (t_fidx){
				.horizontal = (int)(rcintersect.intersection.x),
				.vertical = (int)(round(rcintersect.intersection.y))};
		if (ray.dir.x < 0)
		 	--fieldidx.horizontal;
		if (ray.dir.y < 0)
		 	--fieldidx.vertical;
		
		if (is_wall(pmap, fieldidx))
		{
			wall_found = true;
			if (hitverticalln)
			{
				if (ray.dir.x > 0)
				{
					rcintersect.cubeside = (t_cdir)West;
					rcintersect.impactangle = ray.dir.rad - 0.0 * M_PI;
				}
				else
				{
					rcintersect.cubeside = (t_cdir)East;
					rcintersect.impactangle = ray.dir.rad - 1.0 * M_PI;
				}
			}
			else
			{
				if (ray.dir.y > 0)
				{
					rcintersect.cubeside = (t_cdir)North;
					rcintersect.impactangle = ray.dir.rad - 0.5 * M_PI;
				}
				else
				{
					rcintersect.cubeside = (t_cdir)South;
					rcintersect.impactangle = ray.dir.rad - 1.5 * M_PI;
				}
			}
			if (rcintersect.impactangle > M_PI)
				rcintersect.impactangle -= M_PI;
		}
	}
	return (rcintersect);
}

bool gen_raycast(t_game *const g)
{
	t_player const	p = g->player;
	double const unitdist = cos(g->player.fov);
	double const w2hratio = (double)g->frame.size_x / g->frame.size_y;

	t_line	ray;
	ray.origin = p.pos;
	ray.dir = p.dov;
	for (int idx = 0; idx < g->frame.size_x; ++idx)
	{
		double const rayangle = (p.dov.rad - p.fov / 2.0) + idx * (p.fov / (g->frame.size_x - 1));
		ray.dir = (t_dir){
			.rad = rayangle,
			.x = cos(rayangle),
			.y = sin(rayangle),
		};

		t_rcintersect rcintersect = rayintersection(ray, g->map);
		
		printf("Intersection for ray angle\n");
		double min_dist = rcintersect.dist2intersect * cos(ray.dir.rad - p.dov.rad);
		g->frame.imgcolumn[idx].blockheightpercent =
			min_dist / unitdist * w2hratio;
	}
	return (true);
}

double vnorm(t_coord const *const coord)
{
	return (pow(coord->x * coord->x + coord->y * coord->y, 0.5));
}

bool	update_player_pos(
	t_game *const g, t_coord deltapos, double const deltadov)
{
	t_player *const p = &g->player;

	t_line	ray;
	ray.origin = p->pos;
	ray.dir = p->dov;
	t_rcintersect intersect = rayintersection(ray, g->map);
	double const maxdistbeforeimpact = intersect.dist2intersect
			- p->mindist2wall / cos(intersect.impactangle);
	double const deltadist = vnorm(&deltapos);
	if (deltadist > maxdistbeforeimpact)
	{
		double const ratio = maxdistbeforeimpact / deltadist;
		deltapos.x *= ratio;
		deltapos.y *= ratio;
		g->player.collision = true;
	}
	p->pos.x += deltapos.x * cos(p->dov.rad) + deltapos.y * sin(p->dov.rad);
	p->pos.y += deltapos.x * sin(p->dov.rad) + deltapos.y * cos(p->dov.rad);
	p->dov.rad += deltadov;
	p->pos.x = cos(p->dov.rad);
	p->pos.y = sin(p->dov.rad);
	return (gen_raycast(g));
}