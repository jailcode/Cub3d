#include "../../includes/cub.h"

int    get_current_speed(t_game *data)
{
    int speed;
    speed = 0;
    if (data->key.shift == 1)
}

float next_speed(t_game *data, int speed)
{
    float delta_speed;

    delta_speed = 0;
    if (data->key.w == 1)
        delta_speed = .5;
    if (data->key.shift == 1)
        delta_speed *= 2;
    return (delta_speed);
}