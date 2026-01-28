#include "../../includes/cub.h"

int     alot_key_bind(int keycode, t_game *data)
{
    if (keycode == K_Q || keycode == ESC)
        leave_game(data);
    else if (keycode == K_W)
        data->player.pos.y -= .1;
    else if (keycode == K_A)
        data->player.pos.x -= .1;
    else if (keycode == K_S)
        data->player.pos.y += .1;
    else if (keycode == K_D)
        data->player.pos.x += .1;
    else if (keycode == ARROW_R)
        printf("RIGHT\n");
    else if (keycode == ARROW_L)
        printf("LEFT\n");
    return (0);
}

/* the third parameter of mlx_hook lets us now how to treat the buttons
so 1L<<0 registers input immediately, 1L<<1 only registers on release
the other allowed masks are here:
harm_smits.github.io/42docs/libs/minilibx/events.html*/

void    register_input_hooks(t_game *data)
{
    mlx_key_hook(data->win, &alot_key_bind, data);
    mlx_hook(data->win, 2, 1L<<0, alot_key_bind, data);
}