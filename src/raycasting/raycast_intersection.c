/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_intersection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/02/10 14:10:28 by rhaas            ###   ########.fr       */
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

t_fidx	rayintersection_fieldidx(
	t_gridlns *const pgridlines,
	t_line const ray,
	t_rcintersect	*prcintersect)
{
	t_fidx			fieldidx;

	setup_rayintersection(pgridlines, ray, prcintersect);
	if (prcintersect->hitverticalln)
		fieldidx = (t_fidx){
			.horizontal = (int)(round(prcintersect->intersection.x)),
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

void	rayintersection_horizontal(
	t_line const ray,
	t_rcintersect	*prcintersect)
{
	prcintersect->wallnormal = (t_coord){.x = 1.0, .y = 0.0};
	if (ray.dir.x > 0)
	{
		prcintersect->cubeside = (t_cdir)West;
		prcintersect->relative = fmod(prcintersect->intersection.y,
				1.0);
	}
	else
	{
		prcintersect->cubeside = (t_cdir)East;
		prcintersect->relative = 1.0
			- fmod(prcintersect->intersection.y, 1.0);
	}
}

void	rayintersection_vertical(
	t_line const ray,
	t_rcintersect	*prcintersect)
{
	prcintersect->wallnormal = (t_coord){.x = 0.0, .y = 1.0};
	if (ray.dir.y > 0)
	{
		prcintersect->cubeside = (t_cdir)North;
		prcintersect->relative
			= 1.0 - fmod(prcintersect->intersection.x, 1.0);
	}
	else
	{
		prcintersect->cubeside = (t_cdir)South;
		prcintersect->relative
			= fmod(prcintersect->intersection.x, 1.0);
	}
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
	setup_gridlines(&gridlines, ray);
	wall_found = false;
	while (!wall_found)
	{
		fieldidx = rayintersection_fieldidx(&gridlines, ray, &rcintersect);
		if (is_wall(pmap, fieldidx))
		{
			wall_found = true;
			if (rcintersect.hitverticalln)
				rayintersection_horizontal(ray, &rcintersect);
			else
				rayintersection_vertical(ray, &rcintersect);
		}
	}
	return (rcintersect);
}
