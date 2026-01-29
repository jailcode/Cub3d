#ifndef MINIESSENTIALS_H
# define MINIESSENTIALS_H
# include <X11/keysym.h> // has the keycodes

# include "raycast.h"

# ifndef KEY_LINUX_H
#  define KEY_LINUX_H

#  define ARROW_L 65361
#  define ARROW_U 65362
#  define ARROW_R 65363
#  define ARROW_D 65364 

#  define SPACE 32
#  define ESC 65307
#  define K_Q 113

#  define K_W 119 
#  define K_A 97
#  define K_S 115 
#  define K_D 100

# endif

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

# endif