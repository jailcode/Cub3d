/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:02:47 by pdangwal          #+#    #+#             */
/*   Updated: 2026/02/12 15:02:49 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	init_assets(t_game *data)
{
	data->assets.asset_num = 4;
	data->assets.east.img = NULL;
	data->assets.west.img = NULL;
	data->assets.north.img = NULL;
	data->assets.south.img = NULL;
}

void	init_mouse(t_game *data)
{
	data->input.prev.x = SCREEN_WIDTH / 2;
	data->input.prev.y = SCREEN_HEIGHT / 2;
	data->input.x_diff = 0;
	data->input.y_diff = 0;
}
