/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_math.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/02/10 11:03:32 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

// #include "../includes/cub.h"
#include "raycast.h"
#include <math.h>
#include <stdbool.h>

double	vnorm(t_coord const *const coord)
{
	return (pow(coord->x * coord->x + coord->y * coord->y, 0.5));
}

double	vdot(t_vec const *const v1, t_vec const *const v2)
{
	return (v1->x * v2->x + v1->y * v2->y);
}

double	vcross(t_vec const *const v1, t_vec const *const v2)
{
	return (v1->x * v2->y - v1->y * v2->x);
}

t_vec	transform2csys(t_vec const *const pv, t_vec const *const xaxis)
{
	return ((t_vec){vdot(xaxis, pv), vcross(xaxis, pv)});
}

double	dist(t_coord const *const c1, t_coord const *const c2)
{
	return (hypot(c1->x - c2->x, c1->y - c2->y));
}
