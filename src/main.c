#include "../includes/cub.h"


int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    if (process_map(argv[1]) == true)
        return (1);
    return (0);
}