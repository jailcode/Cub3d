#include "../../includes/cub.h"

t_img	return_asset(t_game *data, t_cdir side)
{
	if (side == East)
		return (data->assets.east);
	else if (side == West)
		return (data->assets.west);
	else if (side == South)
		return (data->assets.south);
	else
		return (data->assets.north);
}

float	normalise(int notnormalised, int start, int end)
{
	return ((float)(notnormalised - start) / (end - start));
}

int	get_pixel_color(t_game *data, t_img *img, t_rccol col,
		float up2downrelative)
{
	char	*color;
	int		img_x;
	int		img_y;

	if (!data || !img)
		return (0);
	img_x = img->size_x * col.left2rightrelative;
	img_y = (img->size_y) * (up2downrelative);
	color = img->addr + (img_y * img->line_length + img_x * (img->bpp / 8));
	return (*(unsigned int *)color);
}



void	put_vertical_line(t_game *data, t_img *tex, t_rccol col)
{
	int	start;
	int	end;
	int	line_length;
	int	color;
	int	start_save;

	line_length = col.blockheightfactor * SCREEN_HEIGHT;
	start_save = ((SCREEN_HEIGHT) * (col.blockstartrelative + 1) - line_length)
		/ 3;
	start = start_save;
	end = start + line_length;
	if (start < 0)
		start = 0;
	paint_ceiling_line(data, start, col.id);
	while (start < end && start < SCREEN_HEIGHT)
	{
		color = get_pixel_color(data, tex, col, normalise(start, start_save,
					end));
		my_mlx_pixel_put(&data->frame, col.id, start, color);
		start++;
	}
	paint_floor_line(data, start, col.id);
}

void	load_world(t_game *data)
{
	int i;
	t_img curr;

	if (!data || !data->frame.imgcolumn)
		return ;
	i = 0;
	while (i < SCREEN_WIDTH)
	{
		data->frame.imgcolumn[i].id = i;
		if (data->frame.imgcolumn[i].blockheightfactor < 0)
			data->frame.imgcolumn[i].blockheightfactor = 0.01;
		curr = return_asset(data, data->frame.imgcolumn[i].cubeside);
		put_vertical_line(data, &curr, data->frame.imgcolumn[i]);
		i++;
	}
}