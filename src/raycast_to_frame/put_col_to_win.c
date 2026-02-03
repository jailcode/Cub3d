#include "../../includes/cub.h"


static int get_wall_color(t_cdir side)
{
    if (side == North)
        return (COLOR_GREEN);
    else if (side == South)
        return (COLOR_BLUE);
    else if (side == East)
        return (COLOR_DARK_GREEN);
    else if (side == West)
        return (COLOR_DARK_BLUE);
    return (COLOR_BLACK);
}
static void    put_vertical_line(t_game *data, int col, float size, t_cdir side)
{
    int line_height;
    int start;
    int end;
    int color;

    if (size <= 0)
        return;
    line_height = (int)(SCREEN_HEIGHT * size);
    start = (SCREEN_HEIGHT - line_height) / 2;
    end = start + line_height;
    while (start < end)
    {
        color = get_wall_color(side);
        my_mlx_pixel_put(&data->frame, col, start, color);
        start++;
    }
}

void    put_cols_to_win(t_game *data)
{
    int i;
    float heightfactor;
    t_cdir side;

    if (!data || !data->frame.imgcolumn)
        return ;    
    i = 0;
    while(i < SCREEN_WIDTH)
    {
        heightfactor = data->frame.imgcolumn[i].blockheightfactor;
        if (heightfactor > 1)
            return ;
        side = data->frame.imgcolumn[i].cubeside;
        put_vertical_line(data, i, heightfactor, side);
        i++;
    }
}