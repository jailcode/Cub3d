#include "cub.h"


void	build_raw_map(t_parser *p)
{
	size_t		i;
	t_map_node	*curr;

	p->map->parse_map = x_malloc(&p->parse_memory, sizeof(char *)
			* (p->map_height + 1));
	curr = p->map_head;
	i = 0;
	while (curr)
	{
		p->map->parse_map[i++] = ft_strtrim(&p->parse_memory, curr->line, "\n");
		curr = curr->next;
	}
	p->map->parse_map[i] = NULL;
	p->map->rows = p->map_height;
	p->map->col = p->map_width;
}

static char	*make_space_line(t_parser *p)
{
	char	*line;
	int		len;
	int		i;

	len = p->map->col + 2;
	line = x_malloc(&p->parse_memory, len + 1);
	if (!line)
		clean_exit(p->parse_memory, 1, "malloc failed");
	i = 0;
	while (i < len)
		line[i++] = ' ';
	line[i] = '\0';
	return (line);
}

static char	*pad_line(t_parser *data, char *line)
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

void	add_map_line(t_parser *p, char *line)
{
	t_map_node	*node;
	size_t		len;

	if (is_empty(line))
		clean_exit(p->parse_memory, 1, "Empty line in map");
	node = x_malloc(&p->parse_memory, sizeof(*node));
	node->line = ft_strdup(&p->parse_memory, line);
	node->next = NULL;
	if (!p->map_head)
		p->map_head = node;
	else
		p->map_tail->next = node;
	p->map_tail = node;
	p->map_height++;
	len = ft_strlen(line);
	if (line[len - 1] == '\n')
		len--;
	if (p->map_width < len)
		p->map_width = len;
}

void	build_padded_map(t_parser *data)
{
	int		old_h;
	int		i;
	char	**new_map;

	if (!data || !data->map || !data->map->parse_map)
		return ;
	old_h = data->map->rows;
	new_map = x_malloc(&data->parse_memory, sizeof(char *) * (old_h + 3));
	if (!new_map)
		clean_exit(data->parse_memory, 1, "malloc failed");
	new_map[0] = make_space_line(data);
	i = 0;
	while (i < old_h)
	{
		new_map[i + 1] = pad_line(data, data->map->parse_map[i]);
		i++;
	}
	new_map[i + 1] = make_space_line(data);
	new_map[i + 2] = NULL;
	data->map->parse_map = new_map;
	data->map->rows = old_h;
	//data->map->col = data->map->col;
}

