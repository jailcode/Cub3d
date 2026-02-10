/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/02/10 08:41:52 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCAST_H
# define RAYCAST_H

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif
# define EPS 1.0E-3

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
	ground
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

// x in width direction (left->right)
// y in height direction (top->down)
typedef struct s_coordinate
{
	double x;
	double y;
}	t_coord;

typedef struct s_screen_cord
{
	int x;
	int y;
} t_screen_coord;

typedef t_coord t_vec;
typedef t_vec t_dir;

// new coord sys specified in the old one as normalised axis vectors
typedef struct s_coordsystem
{
	t_vec	xaxis;
	t_vec	yaxis;
}
t_csys;

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
	double	mindist2wall;
	double	unitdist;
	
	t_coord	pos;
	t_dir	dov;
	double	verticaldovrad;
	bool	collision;
}	t_player;

typedef struct s_gridlines
{
	t_line	horizontal;
	t_line	vertical;
}	t_gridlns;


typedef struct s_rc_intersection
{
	double	dist2intersect;
	double	relative;
	t_coord	intersection;
	t_coord wallnormal;
	t_cdir	cubeside;

}	t_rcintersect;

typedef struct s_rc_image_column
{
	int	id;
	double blockheightfactor;
	double blockstartrelative;
	double left2rightrelative;
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
bool	set_initial_player_pos(t_player *p, t_fidx init_player_field, t_cdir cdcompassdir);

bool	generate_raycast(t_game *const g);

// DeltaDOV [rad] is positive in clockwise direction
// The deltapos is a relative distance:
//   x => to the front/back (positive to the front)
//   y => to the side (positive to the right)
bool	update_player_pos(
	t_game *const g, t_vec deltapos, double const deltadov,
		double const deltaverticaldov);

#endif // RAYCAST_H