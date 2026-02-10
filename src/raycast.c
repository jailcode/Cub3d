/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/02/10 08:33:49 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "../includes/cub.h"
#include "../includes/raycast.h"

#include <math.h>
#include <stdbool.h>

double vnorm(t_coord const *const coord)
{
	return (pow(coord->x * coord->x + coord->y * coord->y, 0.5));
}

double vdot(t_vec const *const v1, t_vec const *const v2)
{
	return (v1->x * v2->x + v1->y * v2->y);
}

double vcross(t_vec const *const v1, t_vec const *const v2)
{
	return (v1->x * v2->y - v1->y * v2->x);
}

// transform a vector to a new csys, the axes of which are defined in the old csys
t_vec	transform2csys(t_vec const *const pv, t_csys const *const pcsys)
{
	// the x-axis would be sufficient here... then the y would need to be
	// calculated using vcross...
	return ((t_vec){
		vdot(pv, &pcsys->xaxis),
		vdot(pv, &pcsys->yaxis)});
}

t_coord	intersect(t_line const *const rayln, t_line const *const gridln)
{
	double const lambda = (
		  rayln->dir.x * (rayln->origin.y - gridln->origin.y)
		- rayln->dir.y * (rayln->origin.x - gridln->origin.x)
	) / (rayln->dir.x * gridln->dir.y - rayln->dir.y * gridln->dir.x);

	return ((t_coord){
		gridln->origin.x + lambda * gridln->dir.x,
		gridln->origin.y + lambda * gridln->dir.y});
}

double dist(t_coord const *const c1, t_coord const *const c2)
{
	return (hypot(c1->x - c2->x, c1->y -c2->y));
}

bool	is_wall(t_map const *const pmap, t_fidx const fidx)
{
	if (!pmap || !pmap->main_map)
		return (false);
	if (fidx.horizontal < 0 || fidx.vertical < 0
		|| fidx.horizontal >= pmap->col || fidx.vertical >= pmap->rows)
	{
		printf("\nDebug: field index out of range, shouldn't happen.\n");
		return (false);
	}
	return ((pmap->main_map)[fidx.vertical][fidx.horizontal].ftype
			== (t_fieldtype)wall);
}

t_rcintersect	rayintersection(t_line const ray, t_map const *const pmap)
{
	t_rcintersect rcintersect;
	bool	wall_found;
	t_gridlns	gridlines = {
		.horizontal = (t_line){.dir = (t_vec){.x = 1.0, .y = 0.0}},
		.vertical   = (t_line){.dir = (t_vec){.x = 0.0, .y = 1.0}},
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
		t_coord const intersect_horizontalln = intersect(&ray, &gridlines.horizontal);
		t_coord const intersect_verticalln = intersect(&ray, &gridlines.vertical);		
		double const dist2hln = dist(&ray.origin, &intersect_horizontalln);
		double const dist2vln = dist(&ray.origin, &intersect_verticalln);
		bool hitverticalln;
		
		if (dist2vln <= dist2hln)
		{
			rcintersect.intersection = intersect_verticalln;
			rcintersect.dist2intersect = dist2vln;				
			hitverticalln = true;
			if (ray.dir.x > 0)
				gridlines.vertical.origin.x += 1.0;
			else
				gridlines.vertical.origin.x -= 1.0;
		}
		else
		{
			rcintersect.intersection = intersect_horizontalln;
			rcintersect.dist2intersect = dist2hln;
			hitverticalln = false;
			if (ray.dir.y > 0)
				gridlines.horizontal.origin.y += 1.0;
			else
				gridlines.horizontal.origin.y -= 1.0;
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
		if (hitverticalln && ray.dir.x < 0)
		 	--fieldidx.horizontal;
		if (!hitverticalln && ray.dir.y < 0)
		 	--fieldidx.vertical;
		if (is_wall(pmap, fieldidx))
		{
			wall_found = true;
			if (hitverticalln)
			{
				rcintersect.wallnormal = (t_coord){ .x = 1.0, .y = 0.0};
				if (ray.dir.x > 0)
				{
					rcintersect.cubeside = (t_cdir)West;
					rcintersect.relative = fmod(rcintersect.intersection.y, 1.0);
				}
				else
				{
					rcintersect.cubeside = (t_cdir)East;
					rcintersect.relative = 1.0 - fmod(rcintersect.intersection.y, 1.0);
				}
			}
			else
			{
				rcintersect.wallnormal = (t_coord){ .x = 0.0, .y = 1.0};
				if (ray.dir.y > 0)
				{
					rcintersect.cubeside = (t_cdir)North;
					rcintersect.relative = 1.0 - fmod(rcintersect.intersection.x, 1.0);
				}
				else
				{
					rcintersect.cubeside = (t_cdir)South;
					rcintersect.relative = fmod(rcintersect.intersection.x, 1.0);
				}
			}
		}
	}
	return (rcintersect);
}

bool gen_raycast(t_game *const g)
{
	t_player const	*const p = &g->player;
	double const	playerdovrad = atan2(p->dov.y, p->dov.x);	
	double const	w2hratio = (double)g->frame.size_x / g->frame.size_y;

	t_line	ray;
	ray.origin = p->pos;
	for (int idx = 0; idx < g->frame.size_x; ++idx)
	{
		t_rccol	*pimgcolumn = &(g->frame.imgcolumn[idx]);
		double const rayangle = (playerdovrad - p->fov / 2.0)
								+ (idx + 0.5) * (p->fov / g->frame.size_x);
		ray.dir = (t_dir){.x = cos(rayangle),.y = sin(rayangle)};
		t_rcintersect rcintersect = rayintersection(ray, g->map);
		
		double min_dist = rcintersect.dist2intersect * vdot(&ray.dir, &p->dov);
		pimgcolumn->blockheightfactor =
			p->unitdist / min_dist * w2hratio;
		pimgcolumn->blockstartrelative = 
			0.5 * (1.0 - pimgcolumn->blockheightfactor)
			+ p->verticaldovrad / p->fov * w2hratio;
		pimgcolumn->cubeside = rcintersect.cubeside;
		pimgcolumn->left2rightrelative = rcintersect.relative;
	}
	return (true);
}

bool set_initial_player_pos(t_game *const g, t_fidx init_player_field, t_cdir const compassdir)
{
	t_player *const p = &g->player;
	
	if (p->pos.x < 0 || p->pos.y < 0)
		return (false);
	p->pos.x += 0.5;
	p->pos.y += 0.5;
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
	return(true);
}

t_csys	maincsysrelative2player(t_player const *const p)
{
	static t_vec const	maincosxaxis = (t_vec){1.0, 0.0};
	t_vec const		xaxis = (t_vec){
		vdot(&p->dov, &maincosxaxis), vcross(&p->dov, &maincosxaxis)};

	return ((t_csys){xaxis, (t_vec){-xaxis.y, xaxis.x}});
}

bool	update_player_pos_internal(
	t_game *const g, t_line ray, double const deltadist)
{
	t_player *const p = &g->player;
	t_rcintersect const intersect = rayintersection(ray, g->map);
	double const maxdistbeforeimpact = 
		intersect.dist2intersect
			- p->mindist2wall / fabs(vdot(&ray.dir, &intersect.wallnormal));
	bool	player_moved = false;
	double	ratio = maxdistbeforeimpact / deltadist;				
	
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
		t_vec wallparallel = (t_vec){-intersect.wallnormal.y, intersect.wallnormal.x};
		double slidedist = deltadist * (1.0 - ratio) * vdot(&ray.dir, &wallparallel);
		if (fabs(slidedist) > EPS)
		{
			ray.dir = wallparallel;
			if (slidedist < 0.0)
			{
				slidedist *= -1.0;
				ray.dir.x *= -1.0;
				ray.dir.y *= -1.0;
			}
			player_moved = update_player_pos_internal(g, ray, slidedist);
		}
	}
	return (player_moved);
}

bool	update_player_pos(
	t_game *const g, t_vec deltapos, double const deltadovrad,
		double const deltaverticalrad)
{
	t_player *const	p = &g->player;
	t_csys const	maincsysrel2plyr = maincsysrelative2player(p);
	t_line			ray;
	bool			player_moved;

	p->collision = false;
	player_moved = false;
	double const deltadist = vnorm(&deltapos);
	if (deltadist > EPS)
	{
		ray.origin = p->pos;
		t_vec const deltaposabs = transform2csys(&deltapos, &maincsysrel2plyr);
		ray.dir.x = deltaposabs.x / deltadist;
		ray.dir.y = deltaposabs.y / deltadist;
		player_moved = update_player_pos_internal(g, ray, deltadist);
	}
	if (fabs(deltadovrad) > EPS)
	{	
		double const	pdovrad = atan2(p->dov.y, p->dov.x) + deltadovrad;			
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
		return(gen_raycast(g), true);
	return (false);
}
