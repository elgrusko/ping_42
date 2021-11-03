# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: ftreand <marvin@le-101.fr>                 +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2018/10/09 14:43:44 by ftreand      #+#   ##    ##    #+#        #
#    Updated: 2018/11/06 19:26:03 by ftreand     ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

.PHONY: all re clean fclean

NAME = ft_ping
HEADER = ft_ping.h
SRC_PATH = ./src/
OBJ_PATH = ./obj/
LIBFT_PATH = ./libft/
LIBFT = $(LIBFT_PATH)libft.a
LIBFT_INC = -L $(LIBFT_PATH) -lft
INCLUDE = -I ./includes/ -I $(LIBFT_PATH)includes

CC = gcc

COLOR = echo

CC_FLAGS = -Wall -Werror -Wextra -g

SRC =	main.c \
		check.c \
		ping.c 

OBJ = $(addprefix $(OBJ_PATH), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	@make -C $(LIBFT_PATH)
	@$(CC) $(CC_FLAGS) $(OBJ) -L $(LIBFT_PATH) -lft -o $(NAME)
	@ printf "\033[1mCompilation du projet \033[34m%-16s$@\033[0m \033[1men \033[31m%-17s$@\033[0m ✅\n"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	@ $(CC) $(CC_FLAGS) $(INCLUDE) -o $@ -c $<
	@ printf "\033[1mCompilation de \033[36m%-25s\033[37m\ten \033[32m%-26s\033[0m ✅\n" $< $@

clean:
	@/bin/rm -rf $(OBJ_PATH)
	@make -C $(LIBFT_PATH) clean

fclean: clean
	@/bin/rm -f $(NAME)
	@make -C $(LIBFT_PATH) fclean

re: fclean all
