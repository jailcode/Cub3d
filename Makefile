NAME := cub3D
UNAME_S := $(shell uname -s)

SRC := src/main.c \
		src/parsing/parse_map.c \
		src/utils/get_next_line.c \
		src/utils/get_line_utils.c \
		src/utils/string_utils.c \
		src/utils/string_utils_2.c \
		src/utils/time.c \
		src/utils/memory_management.c \
		src/init/init.c \
		src/init/init_2.c \
		src/parsing/parse_texture.c \
		src/parsing/build_main_map.c \
		src/parsing/build_parse_map.c \
		src/raycasting/raycast.c \
		src/raycasting/raycast_math.c \
		src/raycasting/raycast_intersection.c \
		src/raycasting/raycast_intersection_helpers.c \
		src/raycasting/raycast_player.c \
		src/verifying/verify_map.c \
		src/verifying/dfs.c \
		src/mlx_essentials/window.c \
		src/mlx_essentials/input.c \
		src/mlx_essentials/minimap.c \
		src/mlx_essentials/color.c \
		src/mlx_essentials/player.c \
		src/mlx_essentials/frame.c \
		src/mlx_essentials/draw_funcs.c \
		src/raycast_to_frame/put_col_to_win.c \
		src/raycast_to_frame/ceiling_floor.c
INCLUDES := includes/cub.h includes/miniessentials.h includes/raycast.h

OBJS := $(SRC:.c=.o)

CC := cc

CFLAGS := -Wall -Werror -Wextra
CFLAGS += -Iminilibx-linux -Iincludes
CFLAGS += -g3

LFLAGS := -lm
MLX_FLAGS = -Lminilibx-linux -lmlx -lXext -lX11

all: $(NAME)

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $(MLX_FLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

norm:
	watch "norminette -R CheckForbiddenSourceHeader $(INCLUDES) $(SRC) | grep Error"

.PHONY: all clean fclean re