NAME := cub3D
UNAME_S := $(shell uname -s)

SRC := src/main.c src/parsing/parse_map.c src/utils/get_next_line.c src/utils/get_line_utils.c \
		src/utils/string_utils.c src/utils/memory_management.c src/init/init.c src/parsing/parse_texture.c \
		src/verifying/verify_map.c src/mlx_essentials/window.c src/mlx_essentials/input.c \
		src/mlx_essentials/minimap.c
INCLUDES := includes/cub.h includes/miniessentials.h includes/raycast.h

OBJS := $(SRC:.c=.o)

CC := cc

FLAGS := -Wall -Werror -Wextra -g

LFLAGS := -lm

MLX_FLAGS = -Lminilibx-linux -lmlx -lXext -lX11

all: $(NAME)

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -Iminilibx-linux -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(LFLAGS) $(MLX_FLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re