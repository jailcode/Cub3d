#include "../includes/cub.h"

int main(int argc, char **argv)
{
    t_game *data; // just data instead of poiner

    if (argc != 2)
        return (1);
    data = init_game();
    if (!data)
        return (1);
    
    if (process_map(data, argv[1]) == false)
        return (1);
    
    clean_memory_list(&data->parse_memory);
    return (0);
}