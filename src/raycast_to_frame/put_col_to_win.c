#include "../../includes/cub.h"


t_img return_asset(t_game *data, t_cdir side)
{
    if (side == East)
        return (data->assets.East);
    else if (side == West)
        return (data->assets.West);
    else if (side == South)
        return (data->assets.South);
    else 
        return data->assets.North;
}



float normalise(int notnormalised, int start, int end)
{
    return ((float)(notnormalised - start) / (float)(end-start));
}

int get_pixel_color(t_game *data, t_img *img, t_rccol col, float up2downrelative)
{
    char *color;
    int img_x;
    int img_y;
    
    if (!data || !img)
        return (0);
    img_x = img->size_x * col.left2rightrelative;
    img_y = img->size_y * up2downrelative;
    color = img->addr + (img_y * img->line_length + img_x * (img->bpp / 8));
    return (*(unsigned int *)color);
}

void    put_vertical_line(t_game *data, t_img *tex, t_rccol col)
{
    int start;
    int end;
    int line_length; // line_length of pixels
    int color;
    int start_save;
 // put one line to the frame;

    line_length = col.blockheightfactor * SCREEN_HEIGHT;
    if (line_length < 0)
        return;
    if (line_length > SCREEN_HEIGHT)
        line_length = SCREEN_HEIGHT;
    start_save = (SCREEN_HEIGHT - line_length) / 2;
    start = start_save;
    end = start + line_length;
    while(start < end)
    {
        color = get_pixel_color(data, tex, col, normalise(start, start_save, end)) ;//gets the color of a relative pixel from a texture 
        my_mlx_pixel_put(&data->frame, col.id, start, color);
        start++;
    }
}

void put_cols_to_win(t_game *data)
{
    int i;
    t_img curr;
    // init col.id
    //to put all the cols from the image onto the frame;

    if (!data || !data->frame.imgcolumn)
        return ;
    i = 0;
    while(i < SCREEN_WIDTH)
    {
        data->frame.imgcolumn[i].id = i;
        curr = return_asset(data, data->frame.imgcolumn[i].cubeside);
        put_vertical_line(data, &curr, data->frame.imgcolumn[i]);
        i++;
    }
}