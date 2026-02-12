#include "../../includes/cub.h"

void	init_frame(t_game *data, t_img *img)
{
	img->size_x = SCREEN_WIDTH;
	img->size_y = SCREEN_HEIGHT;
	img->img = mlx_new_image(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->line_length,
			&img->endian);
}

int	load_frame(t_game *data)
{
	if (!data)
		return (0);
	if (!set_fps(data, TIME_BETWEEN_FRAMES))
		return (0);
	update_player(data);
	load_world(data);
	load_mini_map(data);
	mlx_put_image_to_window(data->mlx, data->win, data->frame.img, 0, 0);
	if (data->key.m == 0)
		mlx_mouse_move(data->mlx, data->win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	return (1);
}
