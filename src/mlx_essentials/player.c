/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 14:55:02 by pdangwal          #+#    #+#             */
/*   Updated: 2026/02/12 14:55:03 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	get_player_info(t_parser *data)
{
	int	i;
	int	j;
	int	ret;

	if (!data)
		return (0);
	i = 0;
	ret = 0;
	while (data->map->parse_map[i])
	{
		j = 0;
		while (data->map->parse_map[i][j])
		{
			if (ft_strchr("NWES", data->map->parse_map[i][j]) != NULL)
			{
				get_player_cdir(data, data->map->parse_map[i][j]);
				data->init_player_field.vertical = i;
				data->init_player_field.horizontal = j;
				ret = 1;
			}
			j++;
		}
		i++;
	}
	return (ret);
}

void	get_player_cdir(t_parser *data, char dov)
{
	if (data->compassdir != cdir_error)
		clean_exit(data->parse_memory, 1, "Multiple player starts");
	if (dov == 'N')
		data->compassdir = North;
	else if (dov == 'S')
		data->compassdir = South;
	else if (dov == 'W')
		data->compassdir = West;
	else if (dov == 'E')
		data->compassdir = East;
}

double	get_mouse_rad(t_game *data, char type)
{
	if (type == 'x')
		return (data->input.x_diff * MOUSE_SENSITIVITY * (M_PI / 180.0));
	else if (type == 'y')
		return (data->input.y_diff * MOUSE_SENSITIVITY * (M_PI / 180.0));
	else
		return (0);
}

void	update_player(t_game *data)
{
	t_coord				delta_pos;
	double				delta_dir;
	double				delta_verticaldir;
	static double const	angularspeed = (M_PI / 2.0) / FPS;

	delta_pos.x = SPEED * (data->key.w - data->key.s);
	delta_pos.y = SPEED * (data->key.d - data->key.a);
	delta_dir = angularspeed * (data->key.right - data->key.left)
		+ get_mouse_rad(data, 'x');
	delta_verticaldir = angularspeed * (data->key.up - data->key.down)
		+ get_mouse_rad(data, 'y');
	data->input.x_diff = 0;
	data->input.y_diff = 0;
	update_player_pos(data, delta_pos, delta_dir, delta_verticaldir);
}
