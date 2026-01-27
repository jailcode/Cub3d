NAME := cub3D
UNAME_S := $(shell uname -s)

SRC := src/main.c src/parsing/parse_map.c src/utils/get_next_line.c src/utils/get_line_utils.c \
		src/utils/string_utils.c src/utils/memory_management.c src/init/init.c src/parsing/parse_texture.c \
		src/verifying/verify_map.c
INCLUDES := includes/cub.h

OBJS := $(SRC:.c=.o)

CC := cc

FLAGS := -Wall -Werror -Wextra -g

LFLAGS := -lm

MLX_FLAGS = -lmlx -lXext -lX11

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -Imlx_linux -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(LFLAGS) $(MLX_FLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re