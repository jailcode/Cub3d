NAME := cub3D

SRC := src/main.c src/parsing/parse_map.c src/utils/get_next_line.c src/utils/get_line_utils.c \
		src/utils/string_utils.c src/utils/memory_management.c src/init/init.c src/parsing/parse_texture.c \
		src/raycast.c

INCLUDES := includes/cub.h includes/raycast.h

OBJS := $(SRC:.c=.o)

CC := cc

FLAGS := -Wall -Werror -Wextra -g

LFLAGS := -lm

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(LFLAGS)

%.o: %.c $(INCLUDES)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re



