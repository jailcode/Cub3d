/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/02/10 11:41:13 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "raycast.h"
#include <stdbool.h>

static void	gen_imgcolumn(t_game *const g, t_rccol *pimgcolumn,
	t_line const *const pray, t_rcintersect const *const prcintersect)
{
	t_player const	*const p = &g->player;
	double const	min_dist = prcintersect->dist2intersect * vdot(&pray->dir, &p->dov);
	double const	w2hratio = (double)g->frame.size_x / g->frame.size_y;
	
	pimgcolumn->blockheightfactor = p->unitdist / min_dist * w2hratio;
	pimgcolumn->blockstartrelative
		= 0.5 * (1.0 - pimgcolumn->blockheightfactor)
		+ p->verticaldovrad / p->fov * w2hratio;
	pimgcolumn->cubeside = prcintersect->cubeside;
	pimgcolumn->left2rightrelative = prcintersect->relative;
}

bool	generate_raycast(t_game *const g)
{
	t_player const	*const p = &g->player;
	double const	playerdovrad = atan2(p->dov.y, p->dov.x);
	t_line			ray;
	double			rayangle;
	t_rcintersect	rcintersect;

	ray.origin = p->pos;
	int	idx = 0;
	while (idx < g->frame.size_x)
	{
		rayangle = (playerdovrad - p->fov / 2.0) + (idx + 0.5) * (p->fov
				/ g->frame.size_x);
		ray.dir = (t_dir){.x = cos(rayangle), .y = sin(rayangle)};
		rcintersect = rayintersection(ray, g->map);
		gen_imgcolumn(g, &(g->frame.imgcolumn[idx]), &ray, &rcintersect);
		++idx;
	}
	return (true);
}
