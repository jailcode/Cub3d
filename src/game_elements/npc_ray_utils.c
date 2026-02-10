#include "../../includes/cub.h"


//to test and learn about rays and implement npcs


typedef struct s_ray
{
    t_cdir side;
    t_fieldtype type;
    float dist;
    float dir;

    float px;
    float py;
    float ny;
    float nx; // initial position
    float step_x;
    float step_y;
    float hit_x;
    float hit_y;
}   t_ray;

/*
t_ray make_ray(t_game *data)
{
    t_ray ret;
    ret.dir = data->player.dov.x;
    ret.px = data->player.pos.x;
    ret.py = data->player.pos.y;
    ret.hit_x = 0;
    ret.hit_y = 0;
    ret.nx = 
    return ret;
}

t_ray cast_ray(t_game *data, t_ray ray)
{

}
*/