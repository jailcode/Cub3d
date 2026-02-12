#include "../../includes/cub.h"

static void	update_cords(t_mmap_dimensions *cords, int i, int j)
{
	cords->top_left.x = 0;
	cords->top_right.y = 0;
	cords->top_right.x = j;
	cords->top_right.y = 0;
	cords->bottom_left.x = 0;
	cords->bottom_left.y = i;
	cords->bottom_right.x = j;
	cords->bottom_right.y = i;
}

void	draw_outline(t_game *data, t_mmap_dimensions *cords)
{
	int	i;
	int	j;

	i = 0;
	while (i < MMAP_TILE_COUNT + 2)
	{
		j = 0;
		while (j < MMAP_TILE_COUNT + 2)
		{
			if (i == 0 || i == MMAP_TILE_COUNT + 1 || j == 0
				|| j == MMAP_TILE_COUNT + 1)
				draw_tile(data, j * TILE_SIZE, i * TILE_SIZE, COLOR_BLACK);
			j++;
		}
		i++;
	}
	(void)cords;
	update_cords(cords, i, j);
}

void	draw_tile(t_game *data, int screen_x, int screen_y, int color)
{
	int	x;
	int	y;

	y = screen_y;
	while (y < screen_y + TILE_SIZE)
	{
		if (y < 0 || y >= SCREEN_HEIGHT)
		{
			y++;
			continue ;
		}
		x = screen_x;
		while (x < screen_x + TILE_SIZE)
		{
			if (x >= 0 && x < SCREEN_WIDTH)
				my_mlx_pixel_put(&data->frame, x, y, color);
			x++;
		}
		y++;
	}
}