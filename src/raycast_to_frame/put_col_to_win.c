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
void    put_vertical_line(t_game *data, int col, float size, t_cdir side)
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

int get_pixel_color(t_img *img, t_coord *text)
{
    int tx = (int)text->x;
    int ty = (int)text->y;

    if (tx < 0) tx = 0;
    if (ty < 0) ty = 0;
    if (tx >= img->size_x) tx = img->size_x - 1;
    if (ty >= img->size_y) ty = img->size_y - 1;
    char *pixel = img->addr + (ty * img->line_length + tx * (img->bpp / 8));
    
    return *(unsigned int *)pixel;
}

// draw a textured vertical line into the frame
void    put_line_from_image(t_game *data, t_img *tex, t_rccol *col, t_coord *tex_pos)
{
    int line_length;
    int start;
    int end;
    int color;
    int i = 0;
    float step;
    if (col->blockheightfactor <= 0) return;
    line_length = (int)(SCREEN_HEIGHT * col->blockheightfactor);
    if (tex->size_y <= 0)
        step = line_length / 1;
    else
        step = line_length / tex->size_y;
    if (step <= 0) step = 1;
    if (line_length > SCREEN_HEIGHT)
        line_length = SCREEN_HEIGHT;
    start = (SCREEN_HEIGHT - line_length) / 2;
    if (start < 0)
        start = 0;
    end = start + line_length;
    // set texture X from column
    tex_pos->y = 0;
    color = get_pixel_color(tex, tex_pos);
    while (start < end)
    {
        if (i >= step)
        {
            i = 0;
            tex_pos->y++;
            color = get_pixel_color(tex, tex_pos);
        }
        my_mlx_pixel_put(&data->frame, col, start, color);
        start++;
        i++;
    }
}

void    split_texture_to_col(t_game *data, t_rccol *col, t_coord *tex_pos)
{
    if (col->cubeside == East)
        put_line_from_image(data, &data->assets.East, col, tex_pos);
    else if (col->cubeside == North)
        put_line_from_image(data, &data->assets.North, col, tex_pos);
    else if (col->cubeside == South)
        put_line_from_image(data, &data->assets.South, col, tex_pos);
    else if (col->cubeside == West)
        put_line_from_image(data, &data->assets.West, col, tex_pos);
}

void    put_cols_to_win(t_game *data)
{
    int i;
    float heightfactor;
    t_coord texture_pos;

    if (!data || !data->frame.imgcolumn)
        return ;    
    i = 0;
    texture_pos.x = 0;
    texture_pos.y = 0;
    while(i < SCREEN_WIDTH)
    {
        heightfactor = data->frame.imgcolumn[i].blockheightfactor;
        if (heightfactor > 1)
            heightfactor = 1;
        data->frame.imgcolumn[i].id = i;
        //put_vertical_line(data, i, heightfactor, side);
        split_texture_to_col(data, &data->frame.imgcolumn[i], &texture_pos);
        //split_texture_to_col(data, i, data->frame.imgcolumn[i]);
        i++;
    }
}