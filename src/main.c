#include "../includes/cub.h"

int main(int argc, char **argv)
{
    t_parser data; // just data instead of poiner

    if (argc != 2)
        return (1);
    init_parser(&data);    
    if (process_map(&data, argv[1]) == false)
        return (1);
    
    clean_memory_list(&data.parse_memory);
    return (0);
}