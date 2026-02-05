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

void    store_img(t_game *data, t_img *img)
{
    img->imgcolumn = NULL;
    img->img = mlx_xpm_file_to_image(data->mlx, img->relative_path, &img->size_x, &img->size_y);
    if (!img->img)
        clean_exit(data->memory, 1, "file does not exist");
    img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->line_length, &img->endian);
    if (!img->addr)
        clean_exit(data->memory, 1, "failed to get image data addr");
}

void    load_assets(t_game *data)
{

    data->assets.North.relative_path = data->map->Ntexture;
    data->assets.East.relative_path = data->map->Etexture;
    data->assets.South.relative_path = data->map->Stexture;
    data->assets.West.relative_path = data->map->Wtexture;
    store_img(data, &data->assets.East);
    store_img(data, &data->assets.West);
    store_img(data, &data->assets.South);
    store_img(data, &data->assets.North);
}