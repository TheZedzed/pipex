# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: azeraoul <azeraoul@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/27 14:19:26 by azeraoul          #+#    #+#              #
#    Updated: 2021/08/29 02:56:19 by azeraoul         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= pipex
CC			:= gcc
CFLAGS		:= -Wall -Werror -Wextra
CPPFLAGS	= -I ./include -I ./libft/include
CPPFLAGS_B	= -I ./libft/include -I./include_bonus
LDFLAGS		:= -L ./libft
LDLIBS		:= -lft
RM			:= rm -rf
SRCS	= $(addprefix ./srcs/, pipex.c init.c error.c)
SRCS_B	= $(addprefix ./srcs_bonus/, pipex_bonus.c init_bonus.c error_bonus.c)
OBJS	:= $(SRCS:.c=.o)
OBJS_B	:= $(SRCS_B:.c=.o)

ifeq ($(BONUS),1)
	CPPFLAGS := $(CPPFLAGS_B)
	SRCS := $(SRCS_B)
	OBJS := $(OBJS_B)
endif

.Phony: all re fclean clean bonus

all: $(NAME)

bonus:
	make BONUS=1

$(NAME): $(OBJS)
	make --silent -C ./libft
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(OBJS_B)
	make clean --silent -C ./libft

fclean: clean
	make fclean --silent -C ./libft
	$(RM) $(NAME)

re: fclean all
