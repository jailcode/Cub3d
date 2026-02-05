/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/02/05 12:11:56 by rhaas            ###   ########.fr       */
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
	t_coord const intersect = (t_coord) {
		gridln->origin.x + lambda * gridln->dir.x,
		gridln->origin.y + lambda * gridln->dir.y};
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
	p->verticaldov = (t_dir){.rad = 0.0, .x = 1.0, .y = 0.0};
	return (true);
}

bool	is_wall(t_map const *const pmap, t_fidx const fidx)
{
	if (!pmap || !pmap->main_map)
		return (false);
	if (fidx.horizontal < 0 || fidx.vertical < 0)
		return (false);
	if (fidx.horizontal >= pmap->col || fidx.vertical >= pmap->rows)
		return (false);
	t_fieldtype const ft = (pmap->main_map)[fidx.vertical][fidx.horizontal].ftype;
	return (ft == (t_fieldtype)wall);
}
/*  added a new is_awll with checks
bool	is_wall(t_map const *const pmap, t_fidx const fidx)
{
	t_field const *const pfield = &pmap->main_map[fidx.horizontal][fidx.vertical];
	return (pfield->ftype == (t_fieldtype)wall);
}*/


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
					rcintersect.impactangle = ray.dir.rad - 0.0 * M_PI;
					rcintersect.relative = fmod(rcintersect.intersection.y, 1.0);
				}
				else
				{
					rcintersect.cubeside = (t_cdir)East;
					rcintersect.impactangle = ray.dir.rad - 1.0 * M_PI;
					rcintersect.relative = 1.0 - fmod(rcintersect.intersection.y, 1.0);
				}
			}
			else
			{
				rcintersect.wallnormal = (t_coord){ .x = 0.0, .y = 1.0};
				if (ray.dir.y > 0)
				{
					rcintersect.cubeside = (t_cdir)North;
					rcintersect.impactangle = ray.dir.rad - 0.5 * M_PI;
					rcintersect.relative = 1.0 - fmod(rcintersect.intersection.x, 1.0);
				}
				else
				{
					rcintersect.cubeside = (t_cdir)South;
					rcintersect.impactangle = ray.dir.rad - 1.5 * M_PI;
					rcintersect.relative = fmod(rcintersect.intersection.x, 1.0);
				}
			}
			rcintersect.impactangle = 
				fmod(rcintersect.impactangle, 2*M_PI);
		}
	}
	return (rcintersect);
}

bool gen_raycast(t_game *const g)
{
	t_player const	*const p = &g->player;
	double const unitdist = cos(p->fov / 2.0);
	double const w2hratio = (double)g->frame.size_x / g->frame.size_y;
	t_rccol	*pimgcolumn;

	t_line	ray;
	ray.origin = p->pos;
	ray.dir = p->dov;
	for (int idx = 0; idx < g->frame.size_x; ++idx)
	{
		pimgcolumn = &(g->frame.imgcolumn[idx]);
		double const rayangle = (p->dov.rad - p->fov / 2.0) + idx * (p->fov / (g->frame.size_x - 1));
		ray.dir = (t_dir){
			.rad = rayangle,
			.x = cos(rayangle),
			.y = sin(rayangle),
		};

		t_rcintersect rcintersect = rayintersection(ray, g->map);
		
		/* DEBUG S
		printf("\nIntersection %d:"
			"\nray x=%.3lf y=%.3lf angle=%.3lf"
			"\nintersect coord x=%.3lf y=%.3lf dist=%.3lf"
			"\n",
			 idx,
			 ray.origin.x, ray.origin.y, rayangle * 180.0 / M_PI,
			 rcintersect.intersection.x, rcintersect.intersection.y,
			 rcintersect.dist2intersect);
		 DEBUG E */
		double min_dist = rcintersect.dist2intersect * cos(ray.dir.rad - p->dov.rad);
		pimgcolumn->blockheightfactor =
			unitdist / min_dist * w2hratio;
		pimgcolumn->blockstartrelative = 
			0.5 * (1.0 - pimgcolumn->blockheightfactor)
			+ p->verticaldov.rad / p->fov * w2hratio;
		pimgcolumn->cubeside = rcintersect.cubeside;
		pimgcolumn->left2rightrelative = rcintersect.relative;
	}
	return (true);
}

double vnorm(t_coord const *const coord)
{
	return (pow(coord->x * coord->x + coord->y * coord->y, 0.5));
}

bool	update_player_pos(
	t_game *const g, t_coord /* const */ deltapos, double const deltadov,
		double const deltaverticaldov)
{
	t_player *const p = &g->player;
	t_line	ray;
	bool	player_moved;

	/* // DEBUG S
	deltapos.x = 0.0;
	deltapos.y = 2.0;
	double tmp = p->pos.x;
	p->pos.x = p->pos.y;
	p->pos.y = tmp;
	// deltadov = 0.0;
	*/ // // DEBUG E

	p->collision = false;
	player_moved = false;
	double const deltadist = vnorm(&deltapos);
	if (deltadist > EPS)
	{
		ray.origin = p->pos;
		ray.dir.rad = fmod(p->dov.rad + atan2(deltapos.y, deltapos.x), 2*M_PI);
		ray.dir.x = cos(ray.dir.rad);
		ray.dir.y = sin(ray.dir.rad);
		t_rcintersect intersect = rayintersection(ray, g->map);
		double const maxdistbeforeimpact = intersect.dist2intersect
				- p->mindist2wall / cos(intersect.impactangle);
		double const ratio = maxdistbeforeimpact / deltadist;				
		if (deltadist > maxdistbeforeimpact) // i.e. ratio < 1.0
		{
			deltapos.x *= ratio;
			deltapos.y *= ratio;
			g->player.collision = true;
		}
		if (vnorm(&deltapos) > EPS)
		{
			p->pos.x += deltapos.x * cos(p->dov.rad) - deltapos.y * sin(p->dov.rad);
			p->pos.y += deltapos.x * sin(p->dov.rad) + deltapos.y * cos(p->dov.rad);
			player_moved = true;
		}
		if (g->player.collision)
		{
			double slidedist = (1.0 - ratio) * deltadist * sin(intersect.impactangle);
			if (fabs(slidedist) > EPS)
			{
				deltapos.x = slidedist * sin(intersect.impactangle);
				deltapos.y = slidedist * cos(intersect.impactangle);
				update_player_pos(g, deltapos, 0.0, 0.0);
				g->player.collision = true;
				player_moved = true;
			}
		}
	}
	if (fabs(deltadov) > EPS)
	{	
		p->dov.rad += deltadov;
		p->dov.x = cos(p->dov.rad);
		p->dov.y = sin(p->dov.rad);
		player_moved = true;
	}
	if (fabs(deltaverticaldov) > EPS)
	{	
		p->verticaldov.rad += deltaverticaldov;
		p->verticaldov.x = cos(p->verticaldov.rad);
		p->verticaldov.y = sin(p->verticaldov.rad);
		player_moved = true;
	}
	if (player_moved)
		gen_raycast(g);
	return (true);
}