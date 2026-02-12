/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 14:55:13 by pdangwal          #+#    #+#             */
/*   Updated: 2026/02/12 14:55:14 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	leave_game(t_game *data)
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

/*
void	set_image_background(t_img *img, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < img->size_y)
	{
		x = 0;
		while (x < img->size_x)
		{
			my_mlx_pixel_put(img, x, y, color);
			x++;
		}
		y++;
	}
}*/

int	set_fps(t_game *data, int time)
{
	long long	new_time;

	new_time = get_time_in_ms();
	if (new_time - data->current_time < time)
		return (0);
	data->current_time = new_time;
	return (1);
}

void	update_loop(t_game *data)
{
	data->current_time = get_time_in_ms();
	mlx_loop_hook(data->mlx, load_frame, data);
	mlx_loop(data->mlx);
}

int	close_window(void *param)
{
	leave_game(param);
	return (0);
}

void	start_game(t_game *data)
{
	if (!data)
		clean_exit(data->memory, 1, "damaged data");
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d");
	init_frame(data, &data->frame);
	load_assets(data);
	mlx_mouse_hide(data->mlx, data->win);
	register_input_hooks(data);
	generate_raycast(data);
	update_loop(data);
}
