#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/time.h>

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
    int     map_width;
    int     map_height;
    char    **map;  //convert t_field

}   t_map;


typedef struct s_mem_list
{
    void				*address;
	struct s_mem_list	*next;
	struct s_mem_list	*prev;
}   t_mem_list;

typedef struct s_map_node
{
    char *line;
    struct s_map_node *next;
}   t_map_node;

typedef enum e_parse_state
{
    PARSE_HEADER,
    PARSE_MAP
} t_parse_state;

typedef struct s_parser
{
    t_map *map;
    t_mem_list *parse_memory;
    t_parse_state   state;
    t_map_node *map_head;
    t_map_node *map_tail;

    size_t map_height;
    size_t map_width;

}   t_parser;

typedef struct s_game
{
    t_map   *map;
    void    *mlx;
    void    *win;
}   t_game;


/*         GET_NEXT_LINE        */

char	*get_next_line(t_mem_list *memory, int fd);
char	*ft_strjoin(t_mem_list *memory, char const *s1, char const *s2);
char	*ft_strchr(const char *string, int searchedChar);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(t_mem_list *memory, size_t elementCount, size_t elementSize);

/*         GET_NEXT_LINE        */


/*      MEMORY MANAGEMENT       */

int	init_memory_list(t_mem_list **m);
void	clean_memory_list(t_mem_list **mem);
void	*x_malloc(t_mem_list **mem, size_t size);
void    clean_exit(t_mem_list *memory, int code, char *message);

/*      MEMORY MANAGEMENT       */


/*            PARSING          */

bool process_map(t_parser *data, char *filename);
void    add_texture_data(char **dest, char *line, t_mem_list **memory);
void add_color_data(int color[3], char *line, t_mem_list **memory);
size_t  ft_strlen(const char *s);
int ft_isspace(char c);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
int ft_isdigit(char c);
int ft_strncmp(const char *s1, const char *s2, size_t len);
char    *ft_strdup(t_mem_list **memory, const char *s);
int    extract_line(t_game *data, char *line);
char	*ft_strtrim(t_mem_list **memory, char const *s1, char const *set);

/*            PARSING          */


/*            INITIALISATION          */

void init_parser(t_parser *data);

/*            INITIALISATION          */


/*          VERFICATION                 */

bool    verify_map(t_parser *data);


/*          VERFICATION                 */
