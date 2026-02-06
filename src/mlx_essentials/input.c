#include "../../includes/cub.h"

int keypress(int keycode, t_game *data)
{
    if (keycode == K_W)
        data->key.w = 1;
    else if (keycode == K_A)
        data->key.a = 1;
    else if (keycode == K_S)
        data->key.s = 1;
    else if (keycode == K_D)
        data->key.d = 1;
    else if (keycode == XK_Left)
        data->key.left = 1;
    else if (keycode == XK_Right)
        data->key.right = 1;
    else if (keycode == XK_Up)
        data->key.up = 1;
    else if (keycode == XK_Down)
        data->key.down = 1;
    return (0);
}

void    destroy_mlx_resources(t_game *data)
{
    mlx_destroy_image(data->mlx, data->frame.img);
    mlx_destroy_window(data->mlx, data->win);
    mlx_destroy_display(data->mlx);
    free(data->mlx);
}

int keyrelease(int keycode, t_game *data)
{
    if (keycode == K_Q || keycode == ESC)
    {
        mlx_loop_end(data->mlx);
        destroy_mlx_resources(data);
        clean_memory_list(&data->memory);
        exit(0);
    }
    if (keycode == K_W)
        data->key.w = 0;
    else if (keycode == K_A)
        data->key.a = 0;
    else if (keycode == K_S)
        data->key.s = 0;
    else if (keycode == K_D)
        data->key.d = 0;
    else if (keycode == XK_Left)
        data->key.left = 0;
    else if (keycode == XK_Right)
        data->key.right = 0;
    else if (keycode == XK_Up)
        data->key.up = 0;
    else if (keycode == XK_Down)
        data->key.down = 0;
    return (0);
}


/* the third parameter of mlx_hook lets us now how to treat the buttons
so 1L<<0 registers input immediately, 1L<<1 only registers on release
the other allowed masks are here:
harm_smits.github.io/42docs/libs/minilibx/events.html*/

void    register_input_hooks(t_game *data)
{
    mlx_hook(data->win, 17, 0, close_window, data);
    mlx_hook(data->win, 2, 1L<<0, keypress, data);
    mlx_hook(data->win, 3, 1L<<1, keyrelease, data);
}