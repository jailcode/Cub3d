/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raphha <raphha@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 11:06:16 by raphha            #+#    #+#             */
/*   Updated: 2026/01/27 13:37:26 by raphha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H

#include "raycast.h"

# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdbool.h>
# include <sys/time.h>

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
    int     width;
    int     height;
    char    **map;
	t_field	**fields;
}   t_map;

typedef struct s_mem_list
{
    void				*address;
	struct s_mem_list	*next;
	struct s_mem_list	*prev;
}   t_mem_list;

typedef struct s_game
{
    t_map   *map;
    t_mem_list *parse_memory;

}   t_game;


/*         GET_NEXT_LINE        */

char	*get_next_line(int fd);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *string, int searchedChar);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t elementCount, size_t elementSize);

/*         GET_NEXT_LINE        */


/*      MEMORY MANAGEMENT       */

int	init_memory_list(t_mem_list **m);
void	clean_memory_list(t_mem_list **mem);
void	*x_malloc(t_mem_list **mem, size_t size);
void    clean_exit(t_mem_list *memory, int code, char *message);

/*      MEMORY MANAGEMENT       */


/*            PARSING          */

bool process_map(t_game *data, char *filename);
size_t  ft_strlen(const char *s);
int ft_isspace(char c);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
int ft_isdigit(char c);
int ft_strncmp(const char *s1, const char *s2, size_t len);
int    extract_line(t_game *data, char *line);

/*            PARSING          */


/*            INITIALISATION          */

t_game *init_game(void);

/*            INITIALISATION          */

#endif // CUB_H