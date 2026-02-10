/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_intersection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/02/10 12:45:09 by rhaas            ###   ########.fr       */
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

void	setup_rayintersection(
	t_gridlns *const pgridlines,
	t_line const ray,
	t_rcintersect	*prcintersect)
{
	t_coord const	intsect_hln = lineintersect(&ray, &pgridlines->horizontal);
	t_coord const	intsect_vln = lineintersect(&ray, &pgridlines->vertical);
	double const	dist2hln = dist(&ray.origin, &intsect_hln);
	double const	dist2vln = dist(&ray.origin, &intsect_vln);

	if (dist2vln <= dist2hln)
	{
		prcintersect->intersection = intsect_vln;
		prcintersect->dist2intersect = dist2vln;
		prcintersect->hitverticalln = true;
		if (ray.dir.x > 0)
			pgridlines->vertical.origin.x += 1.0;
		else
			pgridlines->vertical.origin.x -= 1.0;
	}
	else
	{
		prcintersect->intersection = intsect_hln;
		prcintersect->dist2intersect = dist2hln;
		prcintersect->hitverticalln = false;
		if (ray.dir.y > 0)
			pgridlines->horizontal.origin.y += 1.0;
		else
			pgridlines->horizontal.origin.y -= 1.0;
	}
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

void	setup_gridlines(
	t_gridlns *const pgridlines,
	t_line const ray)
{
	*pgridlines = (t_gridlns){
		.horizontal = (t_line){.dir = (t_vec){.x = 1.0, .y = 0.0}},
		.vertical = (t_line){.dir = (t_vec){.x = 0.0, .y = 1.0}},
	};
	pgridlines->vertical.origin.x = (int)(ray.origin.x);
	if (ray.dir.x > 0)
		pgridlines->vertical.origin.x += 1.0;
	pgridlines->horizontal.origin.y = (int)(ray.origin.y);
	if (ray.dir.y > 0)
		pgridlines->horizontal.origin.y += 1.0;
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
