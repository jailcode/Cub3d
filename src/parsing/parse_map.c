#include "../../includes/cub.h"


void    fill_map(t_game *data, char *line)
{
    char    **map;
    static  int index;

    if (!data || !line)
        clean_exit(data->parse_memory, 1);
    index = 1;
    map = x_malloc(&data->parse_memory, sizeof(*map) * index);
    if (!map)
        clean_exit(&data->parse_memory, 1);
    
 
}

void    pad_map(t_game *data)
{
    if (!data || !data->map->map)
        clean_exit(data->parse_memory, 1);
    
    
}

bool process_map(t_game *data, char *filename)
{
    int     fd;
    char    *line;

    if (!filename)
        return (0);
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (0);
    while((line = get_next_line(fd)) != NULL)
    {
        if (!extract_line(data, line)) // stores everything except for char **map
            fill_map(data, line);
        free(line);
    }
    close(fd);
    pad_map(data);
    return (true);
}