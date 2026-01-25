#include "../../includes/cub.h"

/*
void    fill_map(t_game *data, char *line)
{
    char    **map;
    static  int index;

    if (!data || !line)
        clean_exit(data->parse_memory, 1, "missing data");
    index = 1;
    map = x_malloc(&data->parse_memory, sizeof(*map) * index);
    if (!map)
        clean_exit(data->parse_memory, 1, "malloc failed");
    
 
}*/

int is_map_line(char *line)
{
    int i;

    if (!line || line[0] == '\0')
        return 0;
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


int starts_with(const char *line, const char *id)
{
    int i = 0;

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
    if (!ft_isspace(line[i])) // might have to remove incase the path has alot of spaces
        return (0);
    return (1);
}

int is_empty(char *line)
{
    int i;
    if (!line)
        return (1);
    i = 0;
    while(line[i])
    {
        if (!ft_isspace(line[i]))
            return (0);
        i++;
    }
    return (1);
}

void    add_map_line(t_parser *p, char *line)
{
    t_map_node *node;
    size_t         len;

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

void    parse_header_line(t_parser *p, char *line)
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
        add_texture_data(&p->map->Ntexture, line + 2, &p->parse_memory);
    else if (starts_with(line, "SO"))
        add_texture_data(&p->map->Stexture, line + 2, &p->parse_memory);
    else if (starts_with(line, "WE"))
        add_texture_data(&p->map->Wtexture, line + 2, &p->parse_memory);
    else if (starts_with(line, "EA"))
        add_texture_data(&p->map->Etexture, line + 2, &p->parse_memory);
    else if (starts_with(line, "F"))
        add_color_data(p->map->color_floor, line + 1, &p->parse_memory);
    else if (starts_with(line, "C"))
        add_color_data(p->map->color_ceiling, line + 1, &p->parse_memory);
    else
        clean_exit(p->parse_memory, 1, "Invalid identifier");
}

void    finalize_map(t_parser *p)
{
    int i;
    t_map_node *curr;

    p->map->map = x_malloc(&p->parse_memory,
        sizeof(char *) * (p->map_height + 1));
    curr = p->map_head;
    i = 0;
    while (curr)
    {
        p->map->map[i++] = curr->line;
        curr = curr->next;
    }
    p->map->map[i] = NULL;
    p->map->map_height = p->map_height;
    p->map->map_width = p->map_width;
}

void    pad_map(t_parser *data)
{
    if (!data)
        clean_exit(data->parse_memory, 1, "missing data");
    for (int i = 0; data->map->map[i]; i++)
        printf("%lu and %zu\n", ft_strlen(data->map->map[i]), data->map_width);
}

bool process_map(t_parser *data, char *filename)
{
    int     fd;
    char    *line;

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
        free(line);
    }
    close(fd);
    finalize_map(data);
    pad_map(data);
    return (true);
}