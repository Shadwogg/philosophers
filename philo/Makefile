# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ggiboury <ggiboury@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/05 15:57:04 by ggiboury          #+#    #+#              #
#    Updated: 2023/07/26 00:00:50 by ggiboury         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL = /bin/sh

FLAGS = -Wall -Werror -Wextra -g3

NAME = philo

SRCS = init_philosophers.c	\
		ft_atoi.c			\
		ft_death.c			\
		ft_clock.c			\
		ft_utils.c			\
		ft_free.c			\
		ft_parse.c			\
		ft_print.c			\
		threads.c			\
		philo_life.c

S_MAIN = philosophers.c

OBJS_COMMON = $(SRCS:.c=.o)

OBJ_MAIN = $(S_MAIN:.c=.o)


NC=\033[0m
GREEN=\033[0;32m
RED=\033[0;31m

BOLD=\033[1m


all : $(NAME)

$(OBJS_COMMON) : $(SRCS)
	@gcc $(FLAGS) $? -c

$(OBJ_MAIN) : $(S_MAIN)
	@gcc $(FLAGS) $? -c

$(NAME) : $(OBJS_COMMON) $(OBJ_MAIN)
	@gcc $(FLAGS) $(OBJS_COMMON) $(OBJ_MAIN) -o $(NAME)
	@echo "$(GREEN)philo ready to be executed.$(NC)"

re : fclean $(NAME)

clean :
	@/bin/rm -rf $(OBJS_COMMON)
	@/bin/rm -rf $(OBJ_MAIN)
	@echo ".o files removed."

fclean : clean
	@/bin/rm -f $(NAME)
	@echo "$(GREEN)All compiled files removed.$(NC)"

debug :	$(OBJS_COMMON) $(OBJ_MAIN)
	@gcc $(FLAGS) $(OBJS_COMMON) $(OBJ_MAIN) -o $(NAME)