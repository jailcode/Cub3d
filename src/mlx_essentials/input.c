/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 14:44:29 by pdangwal          #+#    #+#             */
/*   Updated: 2026/02/12 14:49:07 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	toggle_mouse(t_game *data)
{
	if (data->key.m == 1)
	{
		mlx_mouse_hide(data->mlx, data->win);
		data->key.m = 0;
	}
	else if (data->key.m == 0)
	{
		mlx_mouse_show(data->mlx, data->win);
		data->key.m = 1;
	}
}

int	keypress(int keycode, t_game *data)
{
	if (keycode == K_W)
		data->key.w = 1;
	else if (keycode == K_A)
		data->key.a = 1;
	else if (keycode == K_S)
		data->key.s = 1;
	else if (keycode == K_D)
		data->key.d = 1;
	else if (keycode == XK_Left)
		data->key.left = 1;
	else if (keycode == XK_Right)
		data->key.right = 1;
	else if (keycode == XK_Up)
		data->key.up = 1;
	else if (keycode == XK_Down)
		data->key.down = 1;
	else if (keycode == XK_m)
		toggle_mouse(data);
	return (0);
}

void	destroy_mlx_resources(t_game *data)
{
	mlx_destroy_image(data->mlx, data->frame.img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
}

int	keyrelease(int keycode, t_game *data)
{
	if (keycode == K_Q || keycode == ESC)
		leave_game(data);
	if (keycode == K_W)
		data->key.w = 0;
	else if (keycode == K_A)
		data->key.a = 0;
	else if (keycode == K_S)
		data->key.s = 0;
	else if (keycode == K_D)
		data->key.d = 0;
	else if (keycode == XK_Left)
		data->key.left = 0;
	else if (keycode == XK_Right)
		data->key.right = 0;
	else if (keycode == XK_Up)
		data->key.up = 0;
	else if (keycode == XK_Down)
		data->key.down = 0;
	return (0);
}

/* the third parameter of mlx_hook lets us now how to treat the buttons
so 1L<<0 registers input immediately, 1L<<1 only registers on release
the other allowed masks are here:
harm_smits.github.io/42docs/libs/minilibx/events.html*/

void	register_input_hooks(t_game *data)
{
	mlx_hook(data->win, 17, 0, close_window, data);
	mlx_hook(data->win, 2, 1L << 0, keypress, data);
	mlx_hook(data->win, 3, 1L << 1, keyrelease, data);
	mlx_hook(data->win, 6, 1L << 6, mouse_move, data);
}
