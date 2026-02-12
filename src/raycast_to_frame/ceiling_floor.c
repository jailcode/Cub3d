#include "../../includes/cub.h"

void	paint_ceiling_line(t_game *data, int start, int x)
{
	int	i;
	int	color;

	i = 0;
	color = create_rgb(data->map->color_ceiling);
	while (i < start && i < SCREEN_HEIGHT)
	{
		my_mlx_pixel_put(&data->frame, x, i, color);
		i++;
	}
}

void	paint_floor_line(t_game *data, int start, int x)
{
	int	color;

	color = create_rgb(data->map->color_floor);
	while (start < SCREEN_HEIGHT)
	{
		my_mlx_pixel_put(&data->frame, x, start, color);
		start++;
	}
}