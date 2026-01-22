/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raphha <raphha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/01/22 09:56:41 by raphha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int x;
	int y;
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

#include <stdlib.h>
#include <math.h>
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

		
	}
}