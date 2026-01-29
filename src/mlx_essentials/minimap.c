#include "../../includes/cub.h"

void    draw_tile(t_img *frame, int x, int y, int color)
{
    int i;
    int j;

    i = y;
    while(i < y + TILE_SIZE && i < frame->size_y)
    {
        j = x;
        while(j < x + TILE_SIZE && j < frame->size_x)
        {
            my_mlx_pixel_put(frame, j, i, color);
            j++;
        }
        i++;
    }
}
