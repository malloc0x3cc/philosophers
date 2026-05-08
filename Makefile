# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/21 22:49:18 by madelwau          #+#    #+#              #
#    Updated: 2026/05/08 13:50:10 by madelwau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL =	/bin/sh

NAME	= philo

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -I. -lpthread -MMD -MP

# MLX_DIR	= minilibx-linux
# MLX		= $(MLX_DIR)/libmlx.a

# LFT_DIR	= libft
# LFT		= $(LFT_DIR)/libft.a

# ifeq ($(shell uname -s), Darwin)
# 	LIBS	= -L$(MLX_DIR) -lmlx -L /usr/X11/lib -L$(LFT_DIR) \
# 			  -lft -lm -lXext -lX11 -framework OpenGL -framework AppKit
# else
# 	LIBS	= -L$(MLX_DIR) -lmlx -L$(LFT_DIR) -lft -lm -lXext -lX11
# endif

SRCS	= main.c
MODE	= mandatory
# ifeq ($(filter bonus,$(MAKECMDGOALS)),bonus)
# 	SRCS_S	= server_bonus.c
# 	SRCS_C	= client_bonus.c
# 	MODE	= bonus
# else
# 	MODE	= mandatory
# endif

# bonus: all

OBJ_DIR	= .obj/
OBJS	= $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))
DEPS	= $(addprefix $(OBJ_DIR), $(SRCS:.c=.d))

all: $(NAME)

$(MLX):
	@$(MAKE) -C $(MLX_DIR)

$(LFT):
	@$(MAKE) -C $(LFT_DIR)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@printf "$(COL_Y)[DONE] $(NAME) ($(MODE))$(COL_0)\n"

$(OBJ_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I. -c $< -o $@
	@printf "$(COL_G)[OK]$(COL_0) %s -> %s\n" "$<" "$@"

clean:
	# @$(MAKE) clean -C $(MLX_DIR)
	# @$(MAKE) clean -C $(LFT_DIR)
	@rm -rf $(OBJ_DIR)
	@printf "$(COL_R)[RM]$(COL_0) $(shell basename $(CURDIR))/$(OBJ_DIR)\n"

fclean: clean
	# @$(MAKE) fclean -C $(LFT_DIR)
	@rm -f $(NAME)
	@printf "$(COL_R)[RM]$(COL_0) $(COL_B)$(NAME)$(COL_0)\n"

re: fclean all

.PHONY: all clean fclean re
-include $(DEPS)

# Colors
COL_R	= \033[1;31m
COL_G	=\033[1;32m
COL_B	=\033[1;34m
COL_Y	=\033[1;33m
COL_0	=\033[0m
