/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raphha <raphha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/01/21 10:59:29 by raphha           ###   ########.fr       */
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
}	t_field;

typedef struct s_map
{
	// width and height in terms of map fields
	// as parsed from input file (not image pixels)
	int	width; // == no. of block columns
	int height;
	
	t_field	**fields;
}	t_map;

typedef struct s_coordinate
{
	int x; // pixel in width direction (left->right)
	int y; // pixel in height direction (top->down)
}	t_coord;

typedef struct s_player
{
	double dov; // direction of view; angle from x-axis in clockwise direction;
}	t_player;

#include <stdlib.h>

int	main()
{
	t_map	map;
	t_player	p;
	
	map.fields = calloc(map.width, sizeof(t_field *));
	for (int f=0; f<map.width; ++f)
		map.fields[f] = calloc(map.height, sizeof(t_field));
	

}