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
    map->map_height = 0;
    map->map_width = 0;
    return (map); 
}

void init_parser(t_parser *ret)
{
    t_mem_list *memory;

    if (init_memory_list(&memory))
        exit(1);
    ret->map = init_map(&memory);
    if (!ret->map)
        clean_exit(memory, 1, "malloc failed");
    ret->parse_memory = memory;
    ret->map_head = NULL;
    ret->map_tail = NULL;
    ret->map_height = 0;
    ret->map_width = 0;
    ret->player_dir.rad = -1;
    ret->player_pos.x = -1;
    ret->player_pos.y = -1;
    ret->state = PARSE_HEADER;
}

void    init_player(t_player *player, t_coord coords, t_dir dov)
{
    player->dov = dov;
    player->pos = coords;
    player->fov = FOV;
}

void    init_data(t_game *data, t_parser *parser)
{
    data->map = parser->map;  // change to t_map2 or whatever Raphael prefers
    data->memory = parser->parse_memory;
    data->frame.img = NULL;
    data->mlx = NULL;
    data->win = NULL;
    init_player(&data->player, parser->player_pos, parser->player_dir);
}