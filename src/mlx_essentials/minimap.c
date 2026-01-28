#include "../../includes/cub.h"

void    draw_tile(t_img *frame, int x, int y, int color)
{
    int i;
    int j;

    i = y;
    while(i < y + TILE_SIZE && i < frame->size_y)
    {
        j = x;
        while(j < x + TILE_SIZE && j < frame->size_x)
        {
            my_mlx_pixel_put(frame, j, i, color);
            j++;
        }
        i++;
    }
}

int get_color(t_map *map, int row, int col)
{
    char s;

    if (!map || !map->map || row < 0 || col < 0)
        return (0xFFFFFF);
    if (row >= map->map_height || col >= (int)ft_strlen(map->map[row]))
        return (0xFFFFFF);
    s = map->map[row][col];
    if (s == '0')
        return (0x690000);
    else if(s == ' ')
        return (0x696969);
    return (0x696969);
}

void    load_mini_player(t_game *data)
{
    t_coord mini_coord;

    mini_coord.x = SCREEN_HEIGHT - data->player.pos.x;
    mini_coord.y = data->player.pos.y * 3;
    draw_tile(&data->frame, mini_coord.x, mini_coord.y, 0X000000);
}

void    load_mini_map(t_game *data)
{
    int i;
    int j;
    int minimap_size;
    int minimap_tiles;
    int start_y;

    if (!data)
        return;
    minimap_size = SCREEN_WIDTH / 3;
    minimap_tiles = minimap_size / TILE_SIZE;
    start_y = SCREEN_HEIGHT - minimap_size;
    i = 0;
    while(i < minimap_tiles)
    {
        j = 0;
        while(j < minimap_tiles)
        {
            draw_tile(&data->frame, j * TILE_SIZE, i * TILE_SIZE + start_y, get_color(data->map, j, i));
            j++;
        }
        i++;
    }
    load_mini_player(data);
}