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

// RGB Colors for minilibx (0xRRGGBB format)
#  define COLOR_RED 0xFF0000
#  define COLOR_GREEN 0x00FF00
#  define COLOR_BLUE 0x0000FF
#  define COLOR_WHITE 0xFFFFFF
#  define COLOR_BLACK 0x000000
#  define COLOR_YELLOW 0xFFFF00
#  define COLOR_MAGENTA 0xFF00FF
#  define COLOR_CYAN 0x00FFFF
#  define COLOR_ORANGE 0xFFA500
#  define COLOR_PURPLE 0x800080
#  define COLOR_PINK 0xFFC0CB
#  define COLOR_BROWN 0xA52A2A
#  define COLOR_GRAY 0x808080
#  define COLOR_DARK_GRAY 0x404040
#  define COLOR_LIGHT_GRAY 0xC0C0C0
#  define COLOR_DARK_RED 0x800000
#  define COLOR_DARK_GREEN 0x008000
#  define COLOR_DARK_BLUE 0x000080
#  define COLOR_GOLD 0xFFD700
#  define COLOR_LIGHT_GREEN 0x90EE90
#  define COLOR_SKY_BLUE 0x87CEEB

# endif



#endif