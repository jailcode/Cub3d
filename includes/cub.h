/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 11:06:16 by raphha            #+#    #+#             */
/*   Updated: 2026/01/29 18:14:54 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H

# include "raycast.h"
# include "miniessentials.h"
# include "../minilibx-linux/mlx.h"

# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdbool.h>
# include <sys/time.h>
# include <math.h>

# ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
# endif

# ifndef TILE_SIZE
# define TILE_SIZE 24
# endif

# define SCREEN_HEIGHT 600
# define SCREEN_WIDTH  1000
# define FOV 60
# define SPEED .05

# define ANGULAR_SPEED (M_PI / FPS)
# define FPS 60

typedef struct s_map
{
    char    *Ntexture;
    char    *Stexture;
    char    *Wtexture;
    char    *Etexture;
    int     color_floor[3];
    int     color_ceiling[3];
    int     col;
    int     rows;
    char    **parse_map;
    t_field **main_map;

}   t_map;

typedef struct s_mmap_dimensions
{
    t_coord top_left;
    t_coord top_right;
    t_coord bottom_left;
    t_coord bottom_right;
}   t_mmap_dimensions;


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
    t_fidx      init_player_field;
    //t_dir       player_dir;
    t_cdir        compassdir;
    size_t map_height;
    size_t map_width;
    t_rccol     *imgcolumn;
}   t_parser;

typedef struct s_img
{
    void *img;
    char *addr;
    int size_x;
    int size_y;
    int bpp;
    int line_length;
    int endian;
    t_rccol *imgcolumn;
}   t_img;

typedef struct s_keys
{
    int w;
    int a;
    int s;
    int d;
    int left;
    int right;
}   t_keys;

typedef struct s_game
{
    t_map   *map;
    t_player player;
    t_mem_list    *memory;
    t_img   frame;
    t_keys  key;
    void    *mlx;
    void    *win;
    long long current_time;
    t_mmap_dimensions mmap_size;
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
void    init_data(t_game *data, t_parser *parser);

/*            INITIALISATION          */

/*          VERFICATION                 */

bool    verify_map(t_parser *data);

/*          VERFICATION                 */

/*           minilibx funcs             */

void    leave_game(t_game *data);
void    start_game(t_game *data);
void    register_input_hooks(t_game *data);
void    set_image_background(t_img *img, int color); //remove later maybe
void    load_mini_map(t_game *data);
void    my_mlx_pixel_put(t_img *img, int x, int y, int color);
void    put_cols_to_win(t_game *data);
/*           minilibx funcs             */

/*           colors and textures         */
int create_trgb(int t, int r, int g, int b);
/*           colors and textures         */

#endif //CUB_H