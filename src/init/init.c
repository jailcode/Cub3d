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
    map->parse_map = NULL;
    map->main_map = NULL;
    map->rows = 0;
    map->col = 0;
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
    ret->map->main_map = NULL;
    ret->map_tail = NULL;
    ret->map_height = 0;
    ret->map_width = 0;
    ret->compassdir = cdir_error;
    ret->imgcolumn = x_malloc(&memory, SCREEN_WIDTH * sizeof(t_rccol));
    ret->state = PARSE_HEADER;
}

void    init_keys(t_game *data)
{
    data->key.a = 0;
    data->key.d = 0;
    data->key.s = 0;
    data->key.left = 0;
    data->key.right = 0;
    data->key.w = 0;
}

void    init_data(t_game *data, t_parser *parser)
{
    data->map = parser->map;
    data->memory = parser->parse_memory;
    data->frame.img = NULL;
    data->frame.imgcolumn = parser->imgcolumn;
    data->mlx = NULL;
    data->win = NULL;
    data->current_time = 0;
    init_keys(data);
    data->player.fov = FOV * M_PI / 180.0;
	data->player.mindist2wall = 0.2;
    set_initial_player_pos(&data->player, parser->init_player_field, parser->compassdir);
}