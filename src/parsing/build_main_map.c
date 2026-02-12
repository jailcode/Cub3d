#include "cub.h"

t_fieldtype	return_fieldtype(char c)
{
	if (c == '\0')
		return (empty);
	if (c == '0')
		return (ground);
	else if (c == '1')
		return (wall);
	else if (c == ' ')
		return (empty);
	else
		return (ground);
}

void	build_main_map(t_parser *data, t_fieldtype **map)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->map->rows)
	{
		j = -1;
		while (++j < data->map->col)
			map[i][j] /*.ftype*/ = return_fieldtype(data->map->parse_map[i][j]);
	}
}

void copy_main_map(t_parser *data, t_fieldtype **main_map, char **map)
{
	int	i;
	int j;
	int len;

	i = -1;
	while (++i < data->map->rows)
	{
		len = (int)ft_strlen(map[i]);
		j = -1;
		while (++j < data->map->col)
		{
			if (j < len)
				main_map[i][j] /*.ftype*/ = return_fieldtype(map[i][j]);
			else
				main_map[i][j] /*.ftype*/ = empty;
		}
	}
}

void	transfer_map(t_parser *data)
{
	int			i;
	char		**map;
	t_fieldtype	**main_map;

	map = data->map->parse_map;
	main_map = x_malloc(&data->parse_memory, sizeof(*main_map)
			* (data->map->rows));
	i = -1;
	while (++i < data->map->rows)
		main_map[i] = x_malloc(&data->parse_memory, sizeof(**main_map)
				* (data->map->col));
	i = -1;
	copy_main_map(data, main_map, map);
	data->map->main_map = main_map;
}