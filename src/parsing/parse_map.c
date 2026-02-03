#include "../../includes/cub.h"

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

char    *spaced_line(char *line, size_t len)
{
    int i;

    i = -1;
    while((size_t)++i < len)
        line[i] = ' ';
    line[i] = '\0';
    return (line);
}


void    print_map(char **map)
{
    for(int i = 0; map[i]; i++)
         printf("%s\n", map[i]);
}


void    build_raw_map(t_parser *p)
{
    size_t i;
    t_map_node *curr;

    p->map->parse_map = x_malloc(&p->parse_memory,
        sizeof(char *) * (p->map_height + 1));
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
    print_map(p->map->parse_map);
}

char  *pad_line(t_parser *data, char *line)
{
    char *dest;
    int i;
    int     len;

    if (!data || !line)
        return NULL;
    len = data->map->col +  2;// +2 for padded space 
    dest = x_malloc(&data->parse_memory, sizeof(char) * (len + 1)); // 1 for \0, 2 for spaces to pad
    if (!dest)
        clean_exit(data->parse_memory, 1, "malloc failed");
    i = -1;
    while(++i < len)
        dest[i] = ' '; 
    dest[i] = '\0';
    i = 0;
    while(++i < len && line[i - 1])
        dest[i] = line[i - 1];
    return (dest);
}

static char *make_space_line(t_parser *p)
{
    char    *line;
    int     len;
    int     i;

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

void    build_padded_map(t_parser *data)
{
    int old_h;
    int i;
    char    **new_map;

    if (!data || !data->map ||!data->map->parse_map)
        return ;
    old_h = data->map->rows;
    new_map = x_malloc(&data->parse_memory,
                sizeof(char *) * (old_h + 3));
    if (!new_map)
        clean_exit(data->parse_memory, 1, "malloc failed");
    new_map[0] = make_space_line(data);
    i = 0;
    while(i < old_h)
    {
      new_map[i + 1] =  pad_line(data, data->map->parse_map[i]);
        i++;
    }
    new_map[i + 1] = make_space_line(data);
    new_map[i + 2] = NULL;
    data->map->parse_map = new_map;
    data->map->rows = old_h + 2;
    data->map->col = data->map->col + 2;
}

t_fieldtype return_fieldtype(char c)
{
    if (c == '0')
        return (ground);
    else if (c == '1')
        return (wall);
    else if (c == ' ')
        return (empty);
    else return (ground);
}

void    print_main_map(t_map *map)
{
    int i;
    int j;

    if (!map || !map->main_map)
        return ;
    for (i = 0; i < map->rows; i++)
    {
        for (j = 0; j < map->col; j++)
        {
            if (map->main_map[i][j].ftype == wall)
                printf("1");
            else if (map->main_map[i][j].ftype == ground)
                printf("0");
            else if (map->main_map[i][j].ftype == empty)
                printf(" ");
        }
        printf("\n");
    }
}

void    build_main_map(t_parser *data, t_field **map)
{
    int i;
    int j;

    i = -1;
    while(++i < data->map->rows)
    {
        j = -1;
        while(++j < data->map->col)
            map[i][j].ftype = return_fieldtype(data->map->parse_map[i][j]);
    }
}

void    transfer_map(t_parser *data)
{
    int i;
    //char **map;
    t_field **main_map;

    //map = data->map->parse_map;
    main_map = x_malloc(&data->parse_memory, sizeof(*main_map) * (data->map->rows)); // chan
    i = -1;
    while(++i < data->map->rows)
        main_map[i] = x_malloc(&data->parse_memory, sizeof(**main_map) * (data->map->col));
    i = -1;
    build_main_map(data, main_map);
    data->map->main_map = main_map;
    print_main_map(data->map);
}

int get_player_info(t_parser *data);

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
    }
    close(fd);
    build_raw_map(data);
    get_player_info(data);
    build_padded_map(data);
    transfer_map(data);
    return (true);
}