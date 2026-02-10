#include "../../includes/cub.h"


# define MMAP_TILE_COUNT 8
# define MINMAP_SIZE (TILE_SIZE * MMAP_TILE_COUNT)
# define MM_RENDER_DISTANCE 4
#define MMAP_OFFSET_X 24
#define MMAP_OFFSET_Y 24


void draw_tile(t_game *data, int screen_x, int screen_y, int color)
{
    int x;
    int y;

    y = screen_y;
    while (y < screen_y + TILE_SIZE)
    {
        if (y < 0 || y >= SCREEN_HEIGHT)
        {
            y++;
            continue;
        }
        x = screen_x;
        while (x < screen_x + TILE_SIZE)
        {
            if (x >= 0 && x < SCREEN_WIDTH)
                my_mlx_pixel_put(&data->frame, x, y, color);
            x++;
        }
        y++;
    }
}

void    update_cords(t_mmap_dimensions *cords, int i, int j)
{
    cords->top_left.x = 0;
    cords->top_right.y = 0;
    cords->top_right.x = j;
    cords->top_right.y = 0;
    cords->bottom_left.x = 0;
    cords->bottom_left.y = i;
    cords->bottom_right.x = j;
    cords->bottom_right.y = i;
}

void    draw_outline(t_game *data, t_mmap_dimensions *cords)
{
    int i;
    int j;

    i = 0;
    while(i < MMAP_TILE_COUNT + 2)
    {
        j = 0;
        while(j < MMAP_TILE_COUNT + 2)
        {
            if (i == 0 || i == MMAP_TILE_COUNT + 1 || j == 0 || j == MMAP_TILE_COUNT + 1)
                draw_tile(data, j * TILE_SIZE, i * TILE_SIZE, COLOR_BLACK);
            j++;
        }
        i++;
    }
    update_cords(cords, i, j);
}
void    temp_update_player_pos(t_game *data, t_coord delta_pos, double delta_dir);


void    rotate(double *py, double *px, double angle)
{
    *px = *px * cos(angle) - *py * sin(angle);
    *py = *px * sin(angle) + *py * cos(angle);

}

int get_field_color(t_game *data, int row, int col)
{
    if (!data || !data->map || !data->map->main_map)
        return COLOR_BLACK;

    if (row < 0 || col < 0)
        return COLOR_BLACK;

    if (row >= data->map->rows || col >= data->map->col)
        return COLOR_BLACK;

    if (data->map->main_map[row] && data->map->main_map[row][col].ftype == ground)
        return COLOR_BROWN;
    if (data->map->main_map[row] && data->map->main_map[row][col].ftype == wall)
        return COLOR_SKY_BLUE;

    return COLOR_BLACK;
}

void draw_mmap(t_game *data)
{
    int dx;
    int dy;
    int color;
    int screen_x;
    int screen_y;
    int offset_x;
    int offset_y;

    offset_x = ((int)(data->player.pos.x * 100) % 100) * TILE_SIZE / 100;
    offset_y = ((int)(data->player.pos.y * 100) % 100) * TILE_SIZE / 100;
    dy = -MM_RENDER_DISTANCE;
    while (dy <= MM_RENDER_DISTANCE)
    {
        dx = -MM_RENDER_DISTANCE;
        while (dx <= MM_RENDER_DISTANCE)
        {
            color = get_field_color(data, (int)data->player.pos.y + dy,
             (int)data->player.pos.x + dx);
            screen_x = MMAP_OFFSET_X
                + (dx + MM_RENDER_DISTANCE) * TILE_SIZE - offset_x;
            screen_y = MMAP_OFFSET_Y
                + (dy + MM_RENDER_DISTANCE) * TILE_SIZE - offset_y;
            draw_tile(data, screen_x, screen_y, color);
            dx++;
        }
        dy++;
    }
}


void    draw_mini_player(t_game *data)
{
    double mmap_px;
    double mmap_py;

    mmap_px = MM_RENDER_DISTANCE;
    mmap_py = MM_RENDER_DISTANCE;
    //rotate(&mmap_py, &mmap_px, data->player.dov.rad);
    mmap_px = MMAP_OFFSET_X/2 + mmap_px * TILE_SIZE;
    mmap_py = MMAP_OFFSET_Y/2 + mmap_py * TILE_SIZE;
    draw_tile(data, mmap_px, mmap_py, COLOR_DARK_BLUE); // replace with draw_circle
}

void    load_mini_map(t_game *data)
{
    draw_mmap(data);
    draw_outline(data, &data->mmap_size);
    draw_mini_player(data);
}