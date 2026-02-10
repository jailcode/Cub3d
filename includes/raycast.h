/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:41:15 by raphha            #+#    #+#             */
/*   Updated: 2026/02/10 14:13:11 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCAST_H
# define RAYCAST_H

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif
# define EPS 1.0E-3

# include <stdbool.h>

typedef struct s_game	t_game;

typedef enum e_compassdir
{
	North,
	East,
	South,
	West,
	cdir_error
}						t_cdir;

typedef enum e_fieldtype
{
	wall,
	empty,
	ground
}						t_fieldtype;

typedef struct s_fieldindex
{
	int					horizontal;
	int					vertical;
}						t_fidx;

// x in width direction (left->right)
// y in height direction (top->down)
typedef struct s_coordinate
{
	double				x;
	double				y;
}						t_coord;

typedef struct s_screen_cord
{
	int					x;
	int					y;
}						t_screen_coord;

typedef t_coord			t_vec;
typedef t_vec			t_dir;
typedef struct s_line
{
	t_coord				origin;
	t_dir				dir;
}						t_line;

// fov: fixed value in rad, typically 60deg
// dov: direction of view
// pos: exact double coordinates of current position
//      angle from x-axis in clockwise direction
typedef struct s_player
{
	double				fov;
	double				mindist2wall;
	double				unitdist;

	t_coord				pos;
	t_dir				dov;
	double				verticaldovrad;
	bool				collision;
}						t_player;

typedef struct s_gridlines
{
	t_line				horizontal;
	t_line				vertical;
}						t_gridlns;

typedef struct s_rc_intersection
{
	double				dist2intersect;
	double				relative;
	t_coord				intersection;
	t_coord				wallnormal;
	bool				hitverticalln;
	t_cdir				cubeside;

}						t_rcintersect;

typedef struct s_rc_image_column
{
	int					id;
	double				blockheightfactor;
	double				blockstartrelative;
	double				left2rightrelative;
	t_cdir				cubeside;
}						t_rccol;

typedef struct s_raycastresult
{
	t_rccol				*imgcolumn;
	bool				wallcollision;
}						t_rcres;

// the field index is given as a struct with 2 integers,
// cdir is the compass-direction like N(orth) etc.
// starting at 0,0 in the top left corner of the map
bool			set_initial_player_pos(t_player *p,
					t_fidx init_player_field, t_cdir cdcompassdir);
bool			generate_raycast(t_game *const g);

// DeltaDOV [rad] is positive in clockwise direction
// The deltapos is a relative distance:
//   x => to the front/back (positive to the front)
//   y => to the side (positive to the right)
bool			update_player_pos(t_game *const g, t_vec deltapos,
					double const deltadov, double const deltaverticaldov);

// ------------
// raycast_math
// ------------
double			vnorm(t_coord const *const coord);			
double			vdot(t_vec const *const v1, t_vec const *const v2);
double			vcross(t_vec const *const v1, t_vec const *const v2);
double			dist(t_coord const *const c1, t_coord const *const c2);
// transform a vector to a new csys,
// the xaxis of which is defined in the old csys
// The yaxis is rotated by 90° in clockwise direction.
t_vec			transform2csys(t_vec const *const pv, t_vec const *const xaxis);

// --------------------
// raycast_intersection
// --------------------
typedef struct s_map	t_map;
t_rcintersect	rayintersection(t_line const ray, t_map const *const pmap);

t_coord			lineintersect(
					t_line const *const rayln,
					t_line const *const gridln);
void			setup_gridlines(
					t_gridlns *const pgridlines,
					t_line const ray);					
void			setup_rayintersection(
					t_gridlns *const pgridlines,
					t_line const ray,
					t_rcintersect	*prcintersect);

#endif // RAYCAST_H