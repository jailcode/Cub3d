/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 14:52:16 by pdangwal          #+#    #+#             */
/*   Updated: 2026/02/12 14:52:18 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	get_mplayer_offset(t_game *data, char type)
{
	if (type == 'x')
		return (((int)(data->player.pos.x * 100) % 100) * TILE_SIZE / 100);
	else if (type == 'y')
		return (((int)(data->player.pos.y * 100) % 100) * TILE_SIZE / 100);
	else
		return (0);
}

void	draw_mmap(t_game *data)
{
	int	dx;
	int	dy;
	int	color;
	int	screen_x;
	int	screen_y;

	dy = -MM_RENDER_DISTANCE;
	while (dy <= MM_RENDER_DISTANCE)
	{
		dx = -MM_RENDER_DISTANCE;
		while (dx <= MM_RENDER_DISTANCE)
		{
			color = get_field_color(data, (int)data->player.pos.y + dy,
					(int)data->player.pos.x + dx);
			screen_x = MMAP_OFFSET_X + (dx + MM_RENDER_DISTANCE) * TILE_SIZE
				- get_mplayer_offset(data, 'x');
			screen_y = MMAP_OFFSET_Y + (dy + MM_RENDER_DISTANCE) * TILE_SIZE
				- get_mplayer_offset(data, 'y');
			draw_tile(data, screen_x, screen_y, color);
			dx++;
		}
		dy++;
	}
}

void	draw_mini_player(t_game *data)
{
	double	mmap_px;
	double	mmap_py;

	mmap_px = MM_RENDER_DISTANCE;
	mmap_py = MM_RENDER_DISTANCE;
	mmap_px = MMAP_OFFSET_X / 2 + mmap_px * TILE_SIZE;
	mmap_py = MMAP_OFFSET_Y / 2 + mmap_py * TILE_SIZE;
	draw_tile(data, mmap_px, mmap_py, COLOR_DARK_BLUE);
}

void	load_mini_map(t_game *data)
{
	draw_mmap(data);
	draw_outline(data, &data->mmap_size);
	draw_mini_player(data);
}
