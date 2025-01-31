# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/10 17:13:50 by lfiestas          #+#    #+#              #
#    Updated: 2025/01/31 20:36:31 by ljylhank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT
SRCS = main.c minirt.c hooks.c raycast.c vec3.c vec3_normalize.c vec3_scalar.c
OBJS = $(patsubst %.c,build/%.o,$(SRCS))
MLX = MLX42/build/libmlx42.a

CC = cc
CFLAGS = -Iinclude
LFLAGS = -lm -ldl -lglfw -lpthread

MAKEFLAGS += -j6

.PHONY: all release debug clean flcean re

all: release

release: CFLAGS += -O3 -march=native -ffast-math
release: $(NAME)

libft/libft.a:
	make -C libft

$(MLX):
	git clone https://github.com/codam-coding-college/MLX42.git
	cd MLX42 && cmake -B build
	cd MLX42 && cmake --build build -j4

$(NAME): $(OBJS) libft/libft.a $(MLX)
	@mkdir -p build
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

build/%.o: src/%.c src/minirt.h src/vec3.h
	@mkdir -p build
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	make clean -C libft
	rm -rf build

fclean: clean
	make fclean -C libft
	rm -rf build $(NAME) MLX42

re:
	make fclean
	make all

debug: CFLAGS += -ggdb3 -gdwarf
debug: CFLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak
debug: $(NAME)
