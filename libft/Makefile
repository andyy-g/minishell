# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agranger <agranger@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/07/29 09:54:25 by agranger          #+#    #+#              #
#    Updated: 2022/06/23 15:39:19 by agranger         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CFLAGS = -Wall -Wextra -Werror -g 

CC = gcc

SRCS = ft_strlen.c \
	   ft_strdup.c \
	   ft_strndup.c \
	   ft_free.c

OBJS = $(SRCS:.c=.o)

INCLUDES = -I.

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $@ $^
	@echo "$(NAME) created"

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

clean: 
	@$(RM) *.o
	@echo "object files libft removed"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(NAME) removed"

re: fclean all

.PHONY: clean fclean re all bonus
