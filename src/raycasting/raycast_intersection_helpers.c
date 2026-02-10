/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_intersection_helpers.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/02/10 14:10:07 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "raycast.h"
#include <stdbool.h>

t_coord	lineintersect(t_line const *const rayln, t_line const *const gridln)
{
	double const	lambda
		= (rayln->dir.x * (rayln->origin.y - gridln->origin.y)
			- rayln->dir.y * (rayln->origin.x - gridln->origin.x))
		/ (rayln->dir.x	*gridln->dir.y - rayln->dir.y * gridln->dir.x);

	return ((t_coord){gridln->origin.x + lambda * gridln->dir.x,
		gridln->origin.y + lambda * gridln->dir.y});
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
