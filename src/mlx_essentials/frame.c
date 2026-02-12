/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 14:51:12 by pdangwal          #+#    #+#             */
/*   Updated: 2026/02/12 15:27:58 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

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
	static double const TIME_BETWEEN_FRAMES = (1000 / FPS);
	
	if (!data)
		return (0);
	if (!set_fps(data, TIME_BETWEEN_FRAMES))
		return (0);
	update_player(data);
	load_world(data);
	load_mini_map(data);
	mlx_put_image_to_window(data->mlx, data->win, data->frame.img, 0, 0);
	if (data->key.m == 0)
		mlx_mouse_move(data->mlx,
			data->win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	return (1);
}

int	mouse_move(int x, int y, t_game *data)
{
	if (data->key.m == 1)
		return (0);
	data->input.x_diff = (double)(x - SCREEN_WIDTH / 2) / 100.0;
	data->input.y_diff = (double)(SCREEN_HEIGHT / 2 - y) / 50.0;
	data->input.prev.x = x;
	data->input.prev.y = y;
	return (0);
}
