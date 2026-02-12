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

bool	process_map(t_parser *data, char *filename)
{
	int		fd;
	char	*line;

	if (!filename)
		return (false);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (false);
	line = get_next_line(data->parse_memory, fd);
	while (line != NULL)
	{
		if (data->state == PARSE_HEADER)
			parse_header_line(data, line);
		else
			add_map_line(data, line);
		line = get_next_line(data->parse_memory, fd);
	}
	close(fd);
	build_raw_map(data);
	get_player_info(data);
	transfer_map(data);
	build_padded_map(data);
	return (true);
}
