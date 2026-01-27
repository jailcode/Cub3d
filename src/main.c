#include "../includes/cub.h"



int main(int argc, char **argv)
{
    t_parser parser;
    //t_game      data;
    if (argc != 2)
        return (1);
    init_parser(&parser);    
    if (process_map(&parser, argv[1]) == false)
        clean_exit(parser.parse_memory, 1, "file does not exist");
    if (verify_map(&parser) == false)
        clean_exit(parser.parse_memory, 1, "invalid map");
    clean_memory_list(&parser.parse_memory);
    return (0);
}