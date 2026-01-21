#include "../../includes/cub.h"


t_map *init_map(t_mem_list **memory)
{
    int i;
    t_map *map;

    map = x_malloc(memory, sizeof(*map));
    if (!map)
        clean_exit(*memory, 1);
    i = 0;
    while(i < 3)
    {
        map->color_ceiling[i] = -1;
        map->color_floor[i] = -1;
        i++;
    }
    map->Ntexture = NULL;
    map->Stexture = NULL;
    map->Wtexture = NULL;
    map->Etexture = NULL;
    map->map = NULL;
    return (map); 
}

t_game *init_game(void)
{
    t_mem_list *memory;
    t_game *ret;

    if (init_memory_list(&memory))
        return NULL;
    ret = x_malloc(&memory, sizeof(*ret));
    if (!ret)
        clean_exit(memory, 1);
    ret->map = init_map(&memory);
    if (!ret->map)
        clean_exit(memory, 1);
    ret->parse_memory = memory;
    return (ret);
}