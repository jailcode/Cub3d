/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 09:16:08 by rhaas             #+#    #+#             */
/*   Updated: 2026/02/12 12:33:50 by pdangwal         ###   ########.fr       */
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
/*
char	*spaced_line(char *line, size_t len)
{
	int	i;

	i = -1;
	while ((size_t)++i < len)
		line[i] = ' ';
	line[i] = '\0';
	return (line);
}*/

/*
void	print_map(char **map)
{
	size_t		i;
	t_map_node	*curr;

	for (int i = 0; map[i]; i++)
		printf("%s\n", map[i]);
}*/

/*
void	print_main_map(t_map *map)
{
	int	i;
	int	j;

	if (!map || !map->main_map)
		return ;
	for (i = 0; i < map->rows; i++)
	{
		for (j = 0; j < map->col; j++)
		{
			if (map->main_map[i][j] .ftype == wall)
				printf("1");
			else if (map->main_map[i][j] .ftype == ground)
				printf("0");
			else if (map->main_map[i][j] .ftype == empty)
				printf(" ");
		}
		printf("\n");
	}
}
*/

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
