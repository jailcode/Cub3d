/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 11:06:16 by raphha            #+#    #+#             */
/*   Updated: 2026/02/12 15:29:04 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H

# include "mlx.h"
# include "raycast.h"
# include "miniessentials.h"
# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# ifndef TILE_SIZE
#  define TILE_SIZE 24
# endif

# define SCREEN_HEIGHT 600
# define SCREEN_WIDTH 1000
# define FOV 60
# define SPEED .05
# define FPS 60
# define MOUSE_SENSITIVITY 15
# define MMAP_TILE_COUNT 8
# define MM_RENDER_DISTANCE 4
# define MMAP_OFFSET_X 24
# define MMAP_OFFSET_Y 24

typedef struct s_map
{
	char				*n_texture;
	char				*s_texture;
	char				*w_texture;
	char				*e_texture;
	int					color_floor[3];
	int					color_ceiling[3];
	int					col;
	int					rows;
	char				**parse_map;
	t_fieldtype			**main_map;
}						t_map;

typedef struct s_mmap_dimensions
{
	t_coord				top_left;
	t_coord				top_right;
	t_coord				bottom_left;
	t_coord				bottom_right;
}						t_mmap_dimensions;

typedef struct s_mem_list
{
	void				*address;
	struct s_mem_list	*next;
	struct s_mem_list	*prev;
}						t_mem_list;

typedef struct s_map_node
{
	char				*line;
	struct s_map_node	*next;
}						t_map_node;

typedef enum e_parse_state
{
	PARSE_HEADER,
	PARSE_MAP
}						t_parse_state;

typedef struct s_parser
{
	t_map				*map;
	t_mem_list			*parse_memory;
	t_parse_state		state;
	t_map_node			*map_head;
	t_map_node			*map_tail;
	t_fidx				init_player_field;
	t_cdir				compassdir;
	size_t				map_height;
	size_t				map_width;
	t_rccol				*imgcolumn;
}						t_parser;

typedef struct s_img
{
	void				*img;
	char				*relative_path;
	char				*addr;
	int					size_x;
	int					size_y;
	int					bpp;
	int					line_length;
	int					endian;
	t_rccol				*imgcolumn;
}						t_img;

typedef struct s_keys
{
	int					w;
	int					a;
	int					s;
	int					d;
	int					m;
	int					left;
	int					right;
	int					up;
	int					down;
}						t_keys;

typedef struct s_assets
{
	int					asset_num;
	t_img				north;
	t_img				south;
	t_img				east;
	t_img				west;
}						t_assets;

typedef struct s_mouse_input
{
	t_screen_coord		prev;
	double				x_diff;
	double				y_diff;
}						t_mouse;

typedef struct s_game
{
	t_map				*map;
	t_player			player;
	t_mem_list			*memory;
	t_img				frame;
	t_keys				key;
	t_mouse				input;
	t_assets			assets;
	void				*mlx;
	void				*win;
	long long			current_time;
	t_mmap_dimensions	mmap_size;
}						t_game;

/*         GET_NEXT_LINE        */

char					*get_next_line(t_mem_list *memory, int fd);
char					*ft_strjoin(t_mem_list *memory, char const *s1,
							char const *s2);
char					*ft_strchr(const char *string, int searchedChar);
void					ft_bzero(void *s, size_t n);
void					*ft_calloc(t_mem_list *memory, size_t elementCount,
							size_t elementSize);

/*         GET_NEXT_LINE        */

/*      MEMORY MANAGEMENT       */

int						init_memory_list(t_mem_list **m);
void					clean_memory_list(t_mem_list **mem);
void					*x_malloc(t_mem_list **mem, size_t size);
void					clean_exit(t_mem_list *memory, int code, char *message);

/*      MEMORY MANAGEMENT       */

/*            PARSING          */

bool					process_map(t_parser *data, char *filename);
void					add_texture_data(char **dest, char *line,
							t_mem_list **memory);
void					add_color_data(int color[3], char *line,
							t_mem_list **memory);
size_t					ft_strlen(const char *s);
int						ft_isspace(char c);
size_t					ft_strlcpy(char *dest, const char *src, size_t size);
int						ft_isdigit(char c);
int						ft_strncmp(const char *s1, const char *s2, size_t len);
char					*ft_strdup(t_mem_list **memory, const char *s);
int						extract_line(t_game *data, char *line);
char					*ft_strtrim(t_mem_list **memory, char const *s1,
							char const *set);
void					transfer_map(t_parser *data);
int						get_player_info(t_parser *data);
void					paint_floor_line(t_game *data, int start, int x);
void					paint_ceiling_line(t_game *data, int start, int x);
void					build_padded_map(t_parser *data);
void					build_raw_map(t_parser *p);
void					add_map_line(t_parser *p, char *line);
int						is_empty(char *line);
int						no_spaces_inside(t_parser *data, char **map);
void					get_player_cdir(t_parser *data, char dov);
void					dfs(t_parser *data, char **map, int row, int col);
int						load_frame(t_game *data);

/*            PARSING          */

/*            INITIALISATION          */
void					init_assets(t_game *data);
void					init_parser(t_parser *data);
void					init_mouse(t_game *data);
void					init_data(t_game *data, t_parser *parser);

/*            INITIALISATION          */

/*          VERFICATION                 */

bool					verify_map(t_parser *data);

/*          VERFICATION                 */

/*           minilibx funcs             */

void					leave_game(t_game *data);
void					start_game(t_game *data);
void					register_input_hooks(t_game *data);
void					load_mini_map(t_game *data);
void					my_mlx_pixel_put(t_img *img, int x, int y, int color);
void					load_world(t_game *data);
void					load_assets(t_game *data);
int						close_window(void *param);
void					apply_background_color(t_game *data);
int						mouse_input(t_game *data);
int						mouse_move(int x, int y, t_game *data);
int						create_rgb(int color[3]);
int						load_frame(t_game *data);
void					update_player(t_game *data);
int						set_fps(t_game *data, int time);
void					init_frame(t_game *data, t_img *img);
long long				get_time_in_ms(void);
int						get_field_color(t_game *data, int row, int col);
void					draw_tile(t_game *data, int screen_x,
							int screen_y, int color);
void					draw_outline(t_game *data, t_mmap_dimensions *cords);
int						mouse_move(int x, int y, t_game *data);

/*           minilibx funcs             */

/*           colors and textures         */
int						create_trgb(int t, int r, int g, int b);
/*           colors and textures         */

#endif // CUB_H