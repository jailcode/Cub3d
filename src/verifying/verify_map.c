#include "../../includes/cub.h"

void    print_map(char **map)
{
    for(int i = 0; map[i]; i++)
         printf("%s\n", map[i]);
}

int     check_map_elements(t_parser *data)
{
    int i;
    int j;
    t_map   *map;

    if (!data)
        return (0);
    map = data->map;
    i = -1;
    while(++i < map->map_height)
    {
        j = -1;
        while(++j <= map->map_width)
        {
            if (ft_strchr("10 NSWE", map->map[i][j]) == NULL)
                return (0);
        }
    }
    return (1);
}

char    **copy_map(t_mem_list **memory, char **map)
{
    char    **copy;
    int     i;

    if (!map)
        return (NULL);

    i = 0;
    while (map[i])
        i++;
    copy = x_malloc(memory, sizeof(char *) * (i + 1));
    if (!copy)
        clean_exit(*memory, 1, "malloc failed");
    i = 0;
    while (map[i])
    {
        copy[i] = ft_strdup(memory, map[i]);
        if (!copy[i])
            clean_exit(*memory, 1, "malloc failed");
        i++;
    }
    copy[i] = NULL;
    return (copy);
}

void    dfs(t_parser *data, char **map, int row, int col)
{
    if (!data)
        clean_exit(data->parse_memory, 1, "no data");
    if (row < 0 || row >= data->map->map_height || col < 0 || 
        col >= data->map->map_width  + 2|| map[row][col] == '1')
        return ;
    if (map[row][col] && !(map[row][col] == ' ' || map[row][col] == '1'))
        clean_exit(data->parse_memory, 1, "invalid map");

    map[row][col] = '1';
    dfs(data, map, row + 1, col);
    dfs(data, map, row, col + 1);
    dfs(data, map, row - 1, col);
    dfs(data, map, row, col - 1);
}


int no_spaces_inside(t_parser *data, char **map);

int is_closed(t_parser *data)
{
    char    **test_map;

    if (!data)
        return (0);
    test_map = copy_map(&data->parse_memory, data->map->map);
    dfs(data, test_map, 0, 0);
    if (!no_spaces_inside(data, test_map))
        clean_exit(data->parse_memory, 1, "spaces inside map");
    return (1);
}

int no_spaces_inside(t_parser *data, char **map)
{
    int i;
    int j;

    if (!data || !map)
        return (0);
    i = 0;
    while(map[i])
    {
        j = 0;
        while(map[i][j])
        {
            if (map[i][j] == ' ')
                dfs(data, map, i, j);
            j++;
        }
        i++;
    }
    return (1);
}

bool    verify_map(t_parser *data)
{
    if (!data)
        return (false);
    if (!check_map_elements(data))
        return (false);
    if (!is_closed(data))
        return (false);
    return (true);
}