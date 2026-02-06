#include "../../includes/cub.h"


void    leave_game(t_game *data)
{
    mlx_destroy_image(data->mlx, data->frame.img);
    mlx_destroy_image(data->mlx, data->assets.East.img);
    mlx_destroy_image(data->mlx, data->assets.West.img);
    mlx_destroy_image(data->mlx, data->assets.North.img);
    mlx_destroy_image(data->mlx, data->assets.South.img);
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
/*
void    temp_update_player_pos(t_game *data, t_coord delta_pos, double delta_dir)
{
    data->player.pos.x += delta_pos.x/10;
    data->player.pos.y += delta_pos.y/10;
	double const pdovrad = atan2(data->player.dov.y, data->player.dov.x) + delta_dir;
	data->player.dov.x = cos(pdovrad);
	data->player.dov.y = sin(pdovrad);	
}
*/
void    update_player(t_game *data)
{
    t_coord     delta_pos;
    double      delta_dir;
	double      delta_verticaldir;

    delta_pos.x = SPEED *(data->key.w - data->key.s);
    delta_pos.y = SPEED * (data->key.d - data->key.a);
    delta_dir = ANGULAR_SPEED * ( data->key.right - data->key.left) + data->input.x_diff;
	delta_verticaldir = ANGULAR_SPEED * ( data->key.up - data->key.down) * VERTICAL_OFFSET/2 + data->input.y_diff;
    data->input.x_diff = 0;
    data->input.y_diff = 0;
    //mouse_input(data, &delta_dir, &delta_verticaldir);
    update_player_pos(data, delta_pos, delta_dir, delta_verticaldir);

}

int create_rgb(int color[3])
{
    return (color[0] << 16 | color[1] << 8 | color[2]);
}

void    apply_background_color(t_game *data)
{
    int start_ceiling;
    int start_floor;
    int j;
    int *color;

    start_floor = (data->frame.size_y/2) + data->frame.imgcolumn->blockstartrelative * VERTICAL_OFFSET;
    start_ceiling = 0;
    color = data->map->color_ceiling;
    while(start_ceiling < start_floor && start_ceiling <= SCREEN_HEIGHT)
    {
        j = -1;
        while(++j < data->frame.size_x)
            my_mlx_pixel_put(&data->frame, j, start_ceiling, create_rgb(color));
        start_ceiling++; 
    }
    color = data->map->color_floor;
    while(start_floor < data->frame.size_y && start_floor >= 0)
    {
        j = -1;
        while(++j < data->frame.size_x)
            my_mlx_pixel_put(&data->frame, j, start_floor, create_rgb(color));
        start_floor++; 
    }
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