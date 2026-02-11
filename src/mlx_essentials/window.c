#include "../../includes/cub.h"


void    leave_game(t_game *data)
{
    mlx_destroy_image(data->mlx, data->frame.img);
    mlx_destroy_image(data->mlx, data->assets.east.img);
    mlx_destroy_image(data->mlx, data->assets.west.img);
    mlx_destroy_image(data->mlx, data->assets.north.img);
    mlx_destroy_image(data->mlx, data->assets.south.img);
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

void    update_player(t_game *data)
{
    t_coord     delta_pos;
    double      delta_dir;
	double      delta_verticaldir;
	double		mouse_x_rad;
	double		mouse_y_rad;
    static double const angularspeed = (M_PI / 2.0) / FPS;

    delta_pos.x = SPEED *(data->key.w - data->key.s);
    delta_pos.y = SPEED * (data->key.d - data->key.a);
    
    mouse_x_rad = data->input.x_diff * MOUSE_SENSITIVITY * (M_PI / 180.0);
    mouse_y_rad = data->input.y_diff * MOUSE_SENSITIVITY * (M_PI / 180.0);
    
    delta_dir = angularspeed * (data->key.right - data->key.left) + mouse_x_rad;
    delta_verticaldir = angularspeed * (data->key.up - data->key.down) + mouse_y_rad;
    
    data->input.x_diff = 0;
    data->input.y_diff = 0;
    update_player_pos(data, delta_pos, delta_dir, delta_verticaldir);

}

int create_rgb(int color[3])
{
    return (color[0] << 16 | color[1] << 8 | color[2]);
}


int set_fps(t_game *data, int time)
{
    long long new_time;
    new_time = get_time_in_ms();
    if (new_time - data->current_time < time)
        return (0);
    data->current_time = new_time;
    return (1);
}

int load_frame(t_game *data)
{

    if (!data)
        return (0);
    if (!set_fps(data, TIME_BETWEEN_FRAMES))
        return (0);
    update_player(data);
    //apply_background_color(data);
    load_world(data);
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

int close_window(void *param)
{
    leave_game(param);
    return (0);
}

void    start_game(t_game *data)
{
    if (!data)
        clean_exit(data->memory, 1, "damaged data");
    data->mlx = mlx_init();
    data->win = mlx_new_window(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d");
    init_frame(data, &data->frame);
    load_assets(data);
    mlx_mouse_move(data->mlx, data->win, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    data->player.fov = FOV * (M_PI / 180.0);
    if (data->player.mindist2wall <= 0.0)
        data->player.mindist2wall = 0.1;
    register_input_hooks(data);
    update_loop(data);
}