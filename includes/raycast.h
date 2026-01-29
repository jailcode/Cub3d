/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/01/29 11:05:57 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCAST_H
# define RAYCAST_H

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# include <stdbool.h>

typedef struct s_game t_game;

typedef enum e_compassdir
{
	North, East, South, West, cdir_error
}	t_cdir;


typedef enum e_fieldtype
{
	wall,
	empty,
}	t_fieldtype;

typedef struct s_field
{
	t_fieldtype ftype;
}	t_field;

typedef struct s_fieldindex
{
	int horizontal;
	int vertical;
}	t_fidx;

// rad: dov angle in rad
// x,y: Direction of view representation
//      as normalised vector
typedef struct s_direction
{
	double	rad;
	double	x;
	double	y; 
}	t_dir;

// x in width direction (left->right)
// y in height direction (top->down)
typedef struct s_coordinate
{
	double x;
	double y;
}	t_coord;

typedef struct s_line
{
	t_coord	origin;
	t_dir	dir;
}	t_line;

// fov: fixed value in rad, typically 60deg 
// dov: direction of view
// pos: exact double coordinates of current position
//      angle from x-axis in clockwise direction
typedef struct s_player
{
	double	fov;
	t_coord	pos;
	t_dir	dov;
}	t_player;

typedef struct s_gridlines
{
	t_line	horizontal;
	t_line	vertical;
}	t_gridlns;

typedef struct s_rc_image_column
{
	int	ceilpx;
	int cubepx;
	int floorpx;
	t_cdir cubeside;
}	t_rccol;

typedef struct s_raycastresult
{
	t_rccol	*imgcolumn;
	bool	wallcollision;
}	t_rcres;


// the field index is given as a struct with 2 integers,
// cdir is the compass-direction like N(orth) etc.
// starting at 0,0 in the top left corner of the map
bool	set_initial_player_pos(t_player *p, t_fidx init_player_field, t_cdir cdir);

// DeltaDOV is positive in clockwise direction
// for now the deltadov is in rad, but we could
// agree to use deg if you prefer
// The deltapos is a relative distance:
//   x => to the front/back (positive to the front)
//   y => to the side (positive to the right)
t_rcres	update_player_pos(
	t_game *const g, t_coord const deltapos, double const deltadov);

#endif // RAYCAST_H