#include "../../includes/cub.h"

void    add_texture_data(char **dest, char *line, t_mem_list **memory)
{
    int i;
    int len;

    if (!dest || !line)
        return ;
    if (*dest != NULL)
        clean_exit(*memory, 1, "duplicate texture data");
    i = 0;
    while (ft_isspace(line[i]))
        i++;
    if (line[i] == '\0' || line[i] == '\n')
        clean_exit(*memory, 1, "missing path"); //incase path isnt there
    len = 0;
    while (line[i + len] && !ft_isspace(line[i + len]))
        len++;
    *dest = x_malloc(memory, len + 1);
    if (!*dest)
        clean_exit(*memory, 1, "malloc failed");
    ft_strlcpy(*dest, &line[i], len + 1); // copy from start of path til end
}


static int parse_color_value(char *line, int *i, t_mem_list *memory)
{
    int value = 0;

    while (ft_isspace(line[*i]))
        (*i)++;
    if (!ft_isdigit(line[*i]))
        clean_exit(memory, 1, "no numerical input");
    while (ft_isdigit(line[*i]))
        value = value * 10 + (line[(*i)++] - '0');
    if (value < 0 || value > 255)
        clean_exit(memory, 1, "color value out of bounds");
    while (ft_isspace(line[*i]))
        (*i)++;
    return (value);
}

void add_color_data(int color[3], char *line, t_mem_list **memory)
{
    int i;

    if (!color || !line || color[0] != -1)
        clean_exit(*memory, 1, "Duplicate color");
    i = 0;
    color[0] = parse_color_value(line, &i, *memory);
    if (line[i++] != ',')
        clean_exit(*memory, 1, "invalid format");
    color[1] = parse_color_value(line, &i, *memory);
    if (line[i++] != ',')
        clean_exit(*memory, 1, "invalid format");
    color[2] = parse_color_value(line, &i, *memory);
    while (ft_isspace(line[i]))
        i++;
    if (line[i] && line[i] != '\n')
        clean_exit(*memory, 1, "garbage after color");
}

/*
int    extract_line(t_game *data, char *line)
{
    int i;

    if (!data || !line)
        return (0);
    i = 0;
    while(ft_isspace(line[i]))
        i++;
    if (line[i] == '\0' || line[i] == '\n')
        return (0);
    if (ft_strncmp(&line[i], "NO", 2) == 0)
       add_texture_data(&data->map->Ntexture, &line[i + 2], &data->parse_memory);
    else if (ft_strncmp(&line[i], "SO", 2) == 0)
       add_texture_data(&data->map->Stexture, &line[i + 2], &data->parse_memory);
    else if (ft_strncmp(&line[i], "WE", 2) == 0)
       add_texture_data(&data->map->Wtexture, &line[i + 2], &data->parse_memory);
    else if (ft_strncmp(&line[i], "EA", 2) == 0)
       add_texture_data(&data->map->Etexture, &line[i + 2], &data->parse_memory);
    else if (ft_strncmp(&line[i], "F", 1) == 0)
       add_color_data(data->map->color_floor, &line[i + 1], &data->parse_memory);
    else if (ft_strncmp(&line[i], "C", 1) == 0)
       add_color_data(data->map->color_ceiling, &line[i + 1], &data->parse_memory);
    else
        return (0);
    return (1);
}*/