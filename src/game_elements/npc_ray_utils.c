#include "../../includes/cub.h"

/*
//to test and learn about rays and implement npcs


typedef struct s_ray
{
    t_cdir side;
    t_fieldtype type;
    float dist;
    float dir_x;
    float dir_y;

    float px;
    float py;
    float ny;
    float nx; // initial position
    float step_x;
    float step_y;
    float hit_x;
    float hit_y;
    int mapx;
    int mapy;
}   t_ray;


t_ray make_ray(t_game *data)
{
    t_ray ret;
    ret.dir_x = data->player.dov.x;
    ret.dir_y = data->player.dov.y;
    ret.px = data->player.pos.x;
    ret.py = data->player.pos.y;
    ret.hit_x = 0;
    ret.hit_y = 0;
    ret.mapx = (int)ret.px;
    ret.mapy = (int)ret.py;

    return ret;
}

int collided(t_game *data, t_ray *ray)
{
    if (!data || ray)
        return (0);
    
}

t_ray cast_ray(t_game *data, t_ray ray)
{
    int nx;
    int ny;
    t_ray ray;
    double planeX;
    double planeY;

    planeX = 0;
    planeY = .66;
    ray = make_ray(data);
    while(!collided(data, &ray))
    {
        while()
    }
}
*/