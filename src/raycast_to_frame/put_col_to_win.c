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

int get_pixel_color(t_img *img, t_coord *text, t_rccol *col)
{
    int tx;
    int ty;
    char *pixel;
    float rel;

    if (!img || !img->addr)
        return 0;
    rel = col->left2rightrelative;
    if (rel < 0.0f) rel = 0.0f;
    if (rel > 1.0f) rel = 1.0f;

    tx = (int)((img->size_x - 1) * rel);
    if (col->cubeside == East || col->cubeside == South)
        tx = (img->size_x - 1) - tx;
    ty = (int)text->y;
    if (tx < 0) tx = 0;
    if (ty < 0) ty = 0;
    if (tx >= img->size_x)
        tx = img->size_x - 1;
    if (ty >= img->size_y)
        ty = img->size_y - 1;
    pixel = img->addr + (ty * img->line_length + tx * (img->bpp / 8));
    return *(unsigned int *)pixel;
}

void    put_line_from_image(t_game *data, t_img *tex, t_rccol *col, t_coord *tex_pos)
{
    int line_height;
    int start;
    int end;
    int color;
    int step;
    int i;
    if ( col->blockheightfactor <= 0)
        return;
    if (col->blockheightfactor > 1)
        col->blockheightfactor = 1;
    line_height = (int)(SCREEN_HEIGHT * col->blockheightfactor);
    start = (SCREEN_HEIGHT - line_height) / 2;
    end = start + line_height;
    color = get_pixel_color(tex, tex_pos, col);
    tex_pos->y = 0;
    i = 0;
    if (tex->size_y > 0)
        step = line_height/ tex->size_y;
    else step = line_height;
    if (step <= 0) step = 1;
    while (start < end)
    {
        if (i >= step)
        {
            i = 0;
            tex_pos->y++;
            color = get_pixel_color(tex, tex_pos, col);
        }
        my_mlx_pixel_put(&data->frame, col->id, start, color);
        start++;
        i++;
    }
}

void    split_texture_to_col(t_game *data, t_rccol *curr, t_coord *tex_pos)
{
    if (curr->cubeside == East)
        put_line_from_image(data, &data->assets.East, curr, tex_pos);
    else if (curr->cubeside == North)
        put_line_from_image(data, &data->assets.North, curr, tex_pos);
    else if (curr->cubeside == South)
        put_line_from_image(data, &data->assets.South, curr, tex_pos);
    else if (curr->cubeside == West)
        put_line_from_image(data, &data->assets.West, curr, tex_pos);
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
        i++;
    }
}