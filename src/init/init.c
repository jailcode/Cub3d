#include "../../includes/cub.h"


t_map *init_map(t_mem_list **memory)
{
    int i;
    t_map *map;

    map = x_malloc(memory, sizeof(*map));
    if (!map)
        clean_exit(*memory, 1, "malloc failed");
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
    map->height = 0;
    map->width = 0;
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
        clean_exit(memory, 1, "malloc failed");
    ret->map = init_map(&memory);
    if (!ret->map)
        clean_exit(memory, 1, "malloc failed");
    ret->parse_memory = memory;
    return (ret);
}

/*
static t_cdir	char2cdir(char const charcompassdir)
{
	if (charcompassdir == 'N')
		return ((t_cdir)North);
	else if (charcompassdir == 'E')
		return((t_cdir)East);
	else if (charcompassdir == 'S')
		return((t_cdir)South);
	else if (charcompassdir == 'W')
		return((t_cdir)West);
	else
		return ((t_cdir)cdir_error);
}
*/
