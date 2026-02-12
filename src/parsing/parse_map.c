/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 09:16:08 by rhaas             #+#    #+#             */
/*   Updated: 2026/02/12 15:18:20 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	is_map_line(char *line)
{
	int	i;

	if (!line || line[0] == '\0')
		return (0);
	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (!ft_strchr("01NSEW ", line[i]))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	starts_with(const char *line, const char *id)
{
	int	i;

	i = 0;
	if (!line || !id)
		return (0);
	while (ft_isspace(line[i]))
		i++;
	while (*id)
	{
		if (line[i] != *id)
			return (0);
		i++;
		id++;
	}
	if (!ft_isspace(line[i]))
		return (0);
	return (1);
}

int	is_empty(char *line)
{
	int	i;

	if (!line)
		return (1);
	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (0);
		i++;
	}
	return (1);
}

void	parse_header_line(t_parser *p, char *line)
{
	if (!line || is_empty(line))
		return ;
	if (is_map_line(line))
	{
		p->state = PARSE_MAP;
		add_map_line(p, line);
		return ;
	}
	if (starts_with(line, "NO"))
		add_texture_data(&p->map->n_texture, line + 2, &p->parse_memory);
	else if (starts_with(line, "SO"))
		add_texture_data(&p->map->s_texture, line + 2, &p->parse_memory);
	else if (starts_with(line, "WE"))
		add_texture_data(&p->map->w_texture, line + 2, &p->parse_memory);
	else if (starts_with(line, "EA"))
		add_texture_data(&p->map->e_texture, line + 2, &p->parse_memory);
	else if (starts_with(line, "F"))
		add_color_data(p->map->color_floor, line + 1, &p->parse_memory);
	else if (starts_with(line, "C"))
		add_color_data(p->map->color_ceiling, line + 1, &p->parse_memory);
	else
		clean_exit(p->parse_memory, 1, "Invalid identifier");
}

// void	build_raw_map(t_parser *p)
// {
// 	size_t		i;
// 	t_map_node	*curr;

// 	p->map->parse_map = x_malloc(&p->parse_memory, sizeof(char *)
// 			* (p->map_height + 1));
// 	curr = p->map_head;
// 	i = 0;
// 	while (curr)
// 	{
// 		p->map->parse_map[i++] = ft_strtrim(&p->parse_memory, curr->line, "\n");
// 		curr = curr->next;
// 	}
// 	p->map->parse_map[i] = NULL;
// 	p->map->rows = p->map_height;
// 	p->map->col = p->map_width;
// }

char	*pad_line(t_parser *data, char *line)
{
	char	*dest;
	int		i;
	int		len;

	if (!data || !line)
		return (NULL);
	len = data->map->col + 2;
	dest = x_malloc(&data->parse_memory, sizeof(char) * (len + 1));
	if (!dest)
		clean_exit(data->parse_memory, 1, "malloc failed");
	i = -1;
	while (++i < len)
		dest[i] = ' ';
	dest[i] = '\0';
	i = 0;
	while (++i < len && line[i - 1])
		dest[i] = line[i - 1];
	return (dest);
}

// static char	*make_space_line(t_parser *p)
// {
// 	char	*line;
// 	int		len;
// 	int		i;

// 	len = p->map->col + 2;
// 	line = x_malloc(&p->parse_memory, len + 1);
// 	if (!line)
// 		clean_exit(p->parse_memory, 1, "malloc failed");
// 	i = 0;
// 	while (i < len)
// 		line[i++] = ' ';
// 	line[i] = '\0';
// 	return (line);
// }

// void	build_padded_map(t_parser *data)
// {
// 	int		old_h;
// 	int		i;
// 	char	**new_map;

// 	if (!data || !data->map ||!data->map->parse_map)
// 		return ;
// 	old_h = data->map->rows;
// 	new_map = x_malloc(&data->parse_memory,
// 			sizeof(char *) * (old_h + 3));
// 	if (!new_map)
// 		clean_exit(data->parse_memory, 1, "malloc failed");
// 	new_map[0] = make_space_line(data);
// 	i = 0;
// 	while (i < old_h)
// 	{
// 		new_map[i + 1] = pad_line(data, data->map->parse_map[i]);
// 		i++;
// 	}
// 	new_map[i + 1] = make_space_line(data);
// 	new_map[i + 2] = NULL;
// 	data->map->parse_map = new_map;
// 	data->map->rows = old_h;
// 	data->map->col = data->map->col;
// }

// t_fieldtype	return_fieldtype(char c)
// {
// 	if (c == '\0')
// 		return (empty);
// 	if (c == '0')
// 		return (ground);
// 	else if (c == '1')
// 		return (wall);
// 	else if (c == ' ')
// 		return (empty);
// 	else
// 		return (ground);
// }

// void	build_main_map(t_parser *data, t_fieldtype **map)
// {
// 	int	i;
// 	int	j;

// 	i = -1;
// 	while (++i < data->map->rows)
// 	{
// 		j = -1;
// 		while (++j < data->map->col)
// 			map[i][j] = return_fieldtype(data->map->parse_map[i][j]);
// 	}
// }

// void	transfer_map(t_parser *data)
// {
// 	int			i;
// 	int			j;
// 	int			len;
// 	char		**map;
// 	t_fieldtype	**main_map;

// 	map = data->map->parse_map;
// 	main_map = x_malloc(&data->parse_memory, sizeof(*main_map)
// 			* (data->map->rows));
// 	i = -1;
// 	while (++i < data->map->rows)
// 		main_map[i] = x_malloc(&data->parse_memory, sizeof(**main_map)
// 				* (data->map->col));
// 	i = -1;
// 	while (++i < data->map->rows)
// 	{
// 		len = (int)ft_strlen(map[i]);
// 		j = -1;
// 		while (++j < data->map->col)
// 		{
// 			main_map[i][j] = empty;
// 			if (j < len)
// 				main_map[i][j] = return_fieldtype(map[i][j]);
// 		}
// 	}
// 	data->map->main_map = main_map;
// }

// int			get_player_info(t_parser *data);

bool	process_map(t_parser *data, char *filename)
{
	int		fd;
	char	*line;

	if (!filename)
		return (false);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (false);
	while ((line = get_next_line(data->parse_memory, fd)) != NULL)
	{
		if (data->state == PARSE_HEADER)
			parse_header_line(data, line);
		else
			add_map_line(data, line);
	}
	close(fd);
	build_raw_map(data);
	get_player_info(data);
	transfer_map(data);
	build_padded_map(data);
	return (true);
}
