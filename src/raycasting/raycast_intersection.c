/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_intersection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/02/10 12:23:16 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "raycast.h"
#include <stdbool.h>

static bool	is_wall(t_map const *const pmap, t_fidx const fidx)
{
	if (!pmap || !pmap->main_map)
		return (false);
	if (fidx.horizontal < 0 || fidx.vertical < 0 || fidx.horizontal >= pmap->col
		|| fidx.vertical >= pmap->rows)
	{
		printf("\nDebug: field index out of range, shouldn't happen.\n");
		return (false);
	}
	return ((pmap->main_map)[fidx.vertical][fidx.horizontal]
		== (t_fieldtype)wall);
}

t_coord	lineintersect(t_line const *const rayln, t_line const *const gridln)
{
	double const	lambda
		= (rayln->dir.x * (rayln->origin.y - gridln->origin.y)
			- rayln->dir.y * (rayln->origin.x - gridln->origin.x))
		/ (rayln->dir.x	*gridln->dir.y - rayln->dir.y * gridln->dir.x);

	return ((t_coord){gridln->origin.x + lambda * gridln->dir.x,
		gridln->origin.y + lambda * gridln->dir.y});
}

t_fidx	rayintersection_fieldidx(
	t_gridlns *const pgridlines,
	t_line const ray,
	t_rcintersect	*prcintersect)
{
	t_coord const	intersect_horizontalln = lineintersect(&ray,
			&pgridlines->horizontal);
	t_coord const intersect_verticalln = lineintersect(&ray,
			&pgridlines->vertical);
	double			dist2hln;
	double			dist2vln;
	t_fidx			fieldidx;

	dist2hln = dist(&ray.origin, &intersect_horizontalln);
	dist2vln = dist(&ray.origin, &intersect_verticalln);
	if (dist2vln <= dist2hln)
	{
		prcintersect->intersection = intersect_verticalln;
		prcintersect->dist2intersect = dist2vln;
		prcintersect->hitverticalln = true;
		if (ray.dir.x > 0)
			pgridlines->vertical.origin.x += 1.0;
		else
			pgridlines->vertical.origin.x -= 1.0;
	}
	else
	{
		prcintersect->intersection = intersect_horizontalln;
		prcintersect->dist2intersect = dist2hln;
		prcintersect->hitverticalln = false;
		if (ray.dir.y > 0)
			pgridlines->horizontal.origin.y += 1.0;
		else
			pgridlines->horizontal.origin.y -= 1.0;
	}
	if (prcintersect->hitverticalln)
		fieldidx = (t_fidx){
			.horizontal	= (int)(round(prcintersect->intersection.x)),
			.vertical = (int)(prcintersect->intersection.y)};
	else
		fieldidx = (t_fidx){
			.horizontal = (int)(prcintersect->intersection.x),
			.vertical = (int)(round(prcintersect->intersection.y))};
	if (prcintersect->hitverticalln && ray.dir.x < 0)
		--fieldidx.horizontal;
	if (!prcintersect->hitverticalln && ray.dir.y < 0)
		--fieldidx.vertical;
	return (fieldidx);
}

t_rcintersect	rayintersection(t_line const ray, t_map const *const pmap)
{
	t_rcintersect	rcintersect;
	bool			wall_found;
	t_fidx			fieldidx;
	t_gridlns		gridlines;
	
	rcintersect = (t_rcintersect){
		.intersection = (t_coord){.x = 0.0, .y = 0.0},
		.hitverticalln = true};
	gridlines = (t_gridlns){
		.horizontal = (t_line){.dir = (t_vec){.x = 1.0, .y = 0.0}},
		.vertical = (t_line){.dir = (t_vec){.x = 0.0, .y = 1.0}},
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
		fieldidx = rayintersection_fieldidx(&gridlines, ray, &rcintersect);
		if (is_wall(pmap, fieldidx))
		{
			wall_found = true;
			if (rcintersect.hitverticalln)
			{
				rcintersect.wallnormal = (t_coord){.x = 1.0, .y = 0.0};
				if (ray.dir.x > 0)
				{
					rcintersect.cubeside = (t_cdir)West;
					rcintersect.relative = fmod(rcintersect.intersection.y,
							1.0);
				}
				else
				{
					rcintersect.cubeside = (t_cdir)East;
					rcintersect.relative = 1.0
						- fmod(rcintersect.intersection.y, 1.0);
				}
			}
			else
			{
				rcintersect.wallnormal = (t_coord){.x = 0.0, .y = 1.0};
				if (ray.dir.y > 0)
				{
					rcintersect.cubeside = (t_cdir)North;
					rcintersect.relative
						= 1.0 - fmod(rcintersect.intersection.x, 1.0);
				}
				else
				{
					rcintersect.cubeside = (t_cdir)South;
					rcintersect.relative
						= fmod(rcintersect.intersection.x, 1.0);
				}
			}
		}
	}
	return (rcintersect);
}
