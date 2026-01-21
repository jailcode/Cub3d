#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <stdbool.h>

# ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
# endif

typedef struct s_map
{
    char    *Ntexture;
    char    *Stexture;
    char    *Wtexture;
    char    *Etexture;
    int     color_floor[3];
    int     color_ceiling[3];
    char    **map;

}   t_map;

typedef struct s_game
{
    t_map   *map;
}   t_game;


/*         GET_NEXT_LINE        */

char	*get_next_line(int fd);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *string, int searchedChar);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t elementCount, size_t elementSize);

/*         GET_NEXT_LINE        */

/*            PARSING          */

bool process_map(char *filename);
size_t  ft_strlen(const char *s);

/*            PARSING          */

