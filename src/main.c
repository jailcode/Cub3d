#include "../includes/cub.h"


void    print_map(t_parser *p)
{
    for(int i = 0; p->map->map[i]; i++)
         printf("%s\n", p->map->map[i]);
}

int main(int argc, char **argv)
{
    t_parser parser;
    //t_game      data;
    if (argc != 2)
        return (1);
    init_parser(&parser);    
    if (process_map(&parser, argv[1]) == false)
        clean_exit(parser.parse_memory, 1, "file does not exist");
    //if (verify_map(&parser) == false)
        //return (1);
    print_map(&parser);
    clean_memory_list(&parser.parse_memory);
    return (0);
}