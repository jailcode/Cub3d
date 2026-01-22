/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/01/22 14:36:54 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
typedef enum e_fieldtype
{
	wall,
	empty,
}	t_fieldtype;

typedef struct s_field
{
	t_fieldtype ftype;
	// t_coord		pos_idx;
}	t_field;

typedef struct s_map
{
	// width and height in terms of map fields
	// as parsed from input file (not image pixels)
	int	width; // == no. of block columns
	int height;
	
	t_field	**fields;
}	t_map2;

typedef struct s_fieldindex
{
	int horizontal;
	int vertical;
}	t_fidx;

typedef struct s_direction
{
	double	rad; // dov angle in rad
	// Direction of view representation
	// as normalised vector
	double	x;
	double	y; 
}	t_dir;

typedef struct s_coordinate
{
 	// x in width direction (left->right)
 	// y in height direction (top->down)
	// pixel for player position, field indices for field??
	double x;
	double y;
}	t_coord;

typedef struct s_line
{
	t_coord	ref;
	t_dir	dir;
}	t_line;

typedef struct s_player
{
	double	fov;
	t_coord	pos;
	t_dir	dov; // direction of view; angle from x-axis in clockwise direction;
}	t_player;

typedef struct s_gridlines
{
	t_line	horizontal;
	t_line	vertical;
}	t_gridlns;

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

t_coord	intersection(t_line const *const rayln, t_line const *const gridln)
{
	double const lambda = (
		  rayln->dir.x * (rayln->ref.y - gridln->ref.y)
		- rayln->dir.y * (rayln->ref.x - gridln->ref.x)
	) / (rayln->dir.x * gridln->dir.y - rayln->dir.y * gridln->dir.x);
	t_coord const intersect = (t_coord) { gridln->ref.x + lambda * gridln->dir.x, gridln->ref.y + lambda * gridln->dir.y };
	return (intersect);
}

double dist(t_coord const *const c1, t_coord const *const c2)
{
	return (hypot(c1->x - c2->x, c1->y -c2->y));
}
int	main(int argc, char *argv[])
{
	t_map2		map;
	t_player	p = (t_player){.fov = 60.0};
	
	map.fields = calloc(map.width, sizeof(t_field *));
	for (int f=0; f<map.width; ++f)
		map.fields[f] = calloc(map.height, sizeof(t_field));
	// fill map2 from parsing

	/* Player init */
	// idxposition = field idx in map matrix
	t_coord idxposition; // initialised to initial player idxpos on the map
	char	compassdov;	 // initial dov (N,E,S,W)
	set_player_posdov(&p, idxposition, compassdov);

	int	pixelwidth; // = image.width in pxl
	t_gridlns	gridlines = {
		.horizontal = (t_line){.ref = (t_coord){0.0, 0.0}, .dir = (t_dir){.rad = M_PI / 2, .x = 1.0, .y = 0.0}},
		.vertical =  (t_line){.ref = (t_coord){0.0, 0.0}, .dir = (t_dir){.rad = M_PI / 2, .x = 0.0, .y = 1.0}},
	};
	
	for (int idx = 0; idx < pixelwidth; ++idx)
	{
		t_line	ray;
		ray.ref = p.pos;
		double const rayangle = (p.dov.rad - p.fov / 2.0) + idx * (p.fov / (pixelwidth - 1));
		ray.dir = (t_dir){
			.rad = rayangle,
			.x = cos(rayangle),
			.y = sin(rayangle),
		};

		gridlines.vertical.ref.x = (int)(p.pos.x);
		if (ray.dir.x > 0)
			gridlines.vertical.ref.x += 1.0;

		gridlines.horizontal.ref.y = (int)(p.pos.y);
		if (ray.dir.y > 0)
			gridlines.horizontal.ref.y += 1.0;

		bool wall_found = false;
		t_fidx fieldidx;
		t_coord intersect;
		while (!wall_found)
		{
			t_coord intersect_horizontal = intersection(&ray, &gridlines.horizontal);
			t_coord intersect_vertical = intersection(&ray, &gridlines.vertical);		
			double dist_h = dist(&ray.ref, &intersect_horizontal);
			double dist_v = dist(&ray.ref, &intersect_vertical);

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
			if (is_wall(fieldidx))
			{
				wall_found = true;
			}
		}
		
	}
}
