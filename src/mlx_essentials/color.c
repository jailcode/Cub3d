#include "../../includes/cub.h"

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}
int	create_rgb(int color[3])
{
	return (color[0] << 16 | color[1] << 8 | color[2]);
}

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

int	get_field_color(t_game *data, int row, int col)
{
	if (!data || !data->map || !data->map->main_map)
		return (COLOR_BLACK);
	if (row < 0 || col < 0)
		return (COLOR_BLACK);
	if (row >= data->map->rows || col >= data->map->col)
		return (COLOR_BLACK);
	if (data->map->main_map[row] && data->map->main_map[row][col]
		/*.ftype*/ == ground)
		return (COLOR_BROWN);
	if (data->map->main_map[row] && data->map->main_map[row][col]
		/*.ftype*/ == wall)
		return (COLOR_SKY_BLUE);
	return (COLOR_BLACK);
}