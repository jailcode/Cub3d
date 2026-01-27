/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raphha <raphha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/01/27 13:30:16 by raphha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "cub.h"
#include "raycast.h"

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

int	update_player_pos(
	t_player *p, t_coord const deltapos, double const deltadov)
{
	p->pos.x += deltapos.x * cos(p->dov.rad) + deltapos.y * sin(p->dov.rad);
	p->pos.y += deltapos.x * sin(p->dov.rad) + deltapos.y * cos(p->dov.rad);
	p->dov.rad += deltadov;
	p->pos.x = cos(p->dov.rad);
	p->pos.y = sin(p->dov.rad);
}

bool	is_wall(t_map const *const pmap, t_fidx const fidx)
{
	t_field const *const pfield = &pmap->fields[fidx.horizontal][fidx.vertical];
	
	return (pfield->ftype == (t_fieldtype)wall);
}


int	main(int argc, char *argv[])
{
	t_map		map;
	t_player	p = (t_player){.fov = 60.0};
	
	map.fields = calloc(map.width, sizeof(t_field *));
	for (int f=0; f<map.width; ++f)
		map.fields[f] = calloc(map.height, sizeof(t_field));
	// fill map2 from parsing

	/* Player init */
	// idxposition = field idx in map matrix
	t_fidx	idxposition; // initialised to initial player idxpos on the map
	char	compassdov;	 // initial dov (N,E,S,W)

	set_initial_player_pos(&p, idxposition, compassdov);
	int	pixelwidth; // = image.width in pxl
	t_gridlns	gridlines = {
		.horizontal = (t_line){.origin = (t_coord){0.0, 0.0}, .dir = (t_dir){.rad = M_PI / 2, .x = 1.0, .y = 0.0}},
		.vertical =  (t_line){.origin = (t_coord){0.0, 0.0}, .dir = (t_dir){.rad = M_PI / 2, .x = 0.0, .y = 1.0}},
	};
	
	for (int idx = 0; idx < pixelwidth; ++idx)
	{
		t_line	ray;
		ray.origin = p.pos;
		double const rayangle = (p.dov.rad - p.fov / 2.0) + idx * (p.fov / (pixelwidth - 1));
		ray.dir = (t_dir){
			.rad = rayangle,
			.x = cos(rayangle),
			.y = sin(rayangle),
		};

		gridlines.vertical.origin.x = (int)(p.pos.x);
		if (ray.dir.x > 0)
			gridlines.vertical.origin.x += 1.0;

		gridlines.horizontal.origin.y = (int)(p.pos.y);
		if (ray.dir.y > 0)
			gridlines.horizontal.origin.y += 1.0;

		bool wall_found = false;
		t_fidx fieldidx;
		t_coord intersect;
		while (!wall_found)
		{
			t_coord intersect_horizontal = intersection(&ray, &gridlines.horizontal);
			t_coord intersect_vertical = intersection(&ray, &gridlines.vertical);		
			double dist_h = dist(&ray.origin, &intersect_horizontal);
			double dist_v = dist(&ray.origin, &intersect_vertical);

			intersect = intersect_vertical;
			bool hitvertical = true;
			if (dist_h < dist_v)
			{
				intersect = intersect_horizontal;
				hitvertical = false;
			}
			if (hitvertical)
				fieldidx = (t_fidx){.horizontal = (int)(round(intersect.x)), .vertical = (int)(intersect.y)};
			else
				fieldidx = (t_fidx){.horizontal = (int)(intersect.x), .vertical = (int)(round(intersect.y))};
			if (ray.dir.x < 0)
			 	--fieldidx.horizontal;
			if (ray.dir.y < 0)
			 	--fieldidx.vertical;
			if (is_wall(&map, fieldidx))
			{
				wall_found = true;
			}
		}
		printf("Intersection for ray angle\n");
	}
}
