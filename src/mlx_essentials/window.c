#include "../../includes/cub.h"


void    leave_game(t_game *data)
{
    mlx_destroy_image(data->mlx, data->frame.img);
    mlx_destroy_window(data->mlx, data->win);
    mlx_destroy_display(data->mlx);
    free(data->mlx);
    clean_memory_list(&data->memory);
    exit(0);
}


void    my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    char *dst;
    dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
    *(unsigned int *)dst = color;
}

void    set_image_background(t_img *img, int color)
{
    int x;
    int y;

    y = 0;
    while(y < img->size_y)
    {
        x = 0;
        while(x < img->size_x)
        {
            my_mlx_pixel_put(img, x, y, color);
            x++;
        }
        y++;
    }
}

void    init_frame(t_game *data, t_img *img)
{

    img->size_x = SCREEN_WIDTH;
    img->size_y = SCREEN_HEIGHT;
    img->img = mlx_new_image(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->line_length, &img->endian);
}

# define TIME_BETWEEN_FRAMES 1000/FPS

long long get_time_in_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((long long) tv.tv_sec * 1000 + tv.tv_usec/1000);
}

void    temp_update_player_pos(t_game *data, t_coord delta_pos, double delta_dir)
{
    data->player.pos.x += delta_pos.x/10;
    data->player.pos.y += delta_pos.y/10;
    data->player.dov.rad += ANGULAR_SPEED * delta_dir;
}

void    update_player(t_game *data)
{
    t_coord     delta_pos;
    double      delta_dir;

    delta_pos.x = SPEED *(data->key.d - data->key.a);
    delta_pos.y = data->key.s - data->key.w ;
    delta_dir = data->key.right - data->key.left;
    //update_player_pos(data, delta_pos, (const double )delta_dir);
    temp_update_player_pos(data, delta_pos, delta_dir);
}



int load_frame(t_game *data)
{
    long long new_time;

    if (!data)
        return (0);
    
    new_time = get_time_in_ms();
    if (new_time - data->current_time < TIME_BETWEEN_FRAMES)
        return (0);
    data->current_time = new_time;
    update_player(data);
    set_image_background(&data->frame, 0xFFFFFF);
    load_mini_map(data);
    mlx_put_image_to_window(data->mlx, data->win, data->frame.img, 0, 0);
    return (1);
}

void    update_loop(t_game *data)
{
    data->current_time = get_time_in_ms();
    mlx_loop_hook(data->mlx, load_frame, data);
    mlx_loop(data->mlx);
}

void    start_game(t_game *data)
{
    if (!data)
        clean_exit(data->memory, 1, "damaged data");
    data->mlx = mlx_init();
    data->win = mlx_new_window(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d");
    init_frame(data, &data->frame);
    register_input_hooks(data);
    update_loop(data);
}