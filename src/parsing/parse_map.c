#include "../../includes/cub.h"


bool process_map(char *filename)
{
    int     fd;
    char    *line;

    if (!filename)
        return (0);
    fd = open(filename, O_RDONLY);
    while((line = get_next_line(fd)) > 0)
    {
        printf("%s", line);
        //parse_line(line);
        free(line);
    }
    return (true);
}