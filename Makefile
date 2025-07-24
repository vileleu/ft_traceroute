# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/26 14:12:47 by vileleu           #+#    #+#              #
#    Updated: 2025/07/24 13:51:49 by vileleu          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BLUE		= \033[0;34m
GREEN		= \033[0;32m
LIGHTBLUE	= \033[1;34m
RED			= \033[0;31m
YELLOW		= \033[1;33m
ORANGE		= \033[0;33m
MAGENTA		= \033[0;35m
RESET		= \033[0m

DIR_SRCS	= srcs
DIR_INCS	= includes
DIR_OBJS	= objects
DIR_DEPS 	= dependencies
DIR_LIBS	= libraries

SRCS		=	main.c functions_data.c parse.c \
				functions_socket.c error.c utils.c

INCS		= -I $(DIR_INCS)
OBJS 		= $(patsubst %.c,$(DIR_OBJS)/%.o,$(SRCS))
DEPS 		= $(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$(OBJS))

NAME		= ft_traceroute
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
OFLAGS		= -MMD -MP -MF $(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$@)
RM			= rm -rf

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c
			@mkdir -p $(dir $@) $(patsubst $(DIR_OBJS)/%,$(DIR_DEPS)/%,$(dir $@))
			@printf "\n$(BLUE)$< -> $(ORANGE)$@ $(BLUE)-> $(ORANGE)$(patsubst $(DIR_OBJS)/%.o,$(DIR_DEPS)/%.d,$@)$(RESET)"
			@$(CC) $(CFLAGS) $(OFLAGS) $(INCS) -c $< -o $@

$(NAME):	$(OBJS)
			@printf "\n\n$(BLUE)Compiling $(NAME) ... $(RESET)"
			@$(CC) $(CFLAGS) $(OBJS) $(INCS) -o $(NAME)
			@printf "$(GREEN)[✔]\n[$(NAME) done]$(RESET)\n\n"

-include	$(DEPS)

all:		 $(NAME)

clean:
			@printf "\n$(BLUE)Clean objects and dependencies ..."
			@$(RM) $(DIR_OBJS) $(DIR_DEPS)
			@printf "$(GREEN) [✔]$(RESET)\n\n"

fclean:		clean
			@printf "$(BLUE)Delete $(NAME) ..."
			@$(RM) $(NAME)
			@printf "$(GREEN) [✔]$(RESET)\n\n"

re:			fclean all

.PHONY:		all clean fclean re