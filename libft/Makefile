# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agranger <agranger@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/07/29 09:54:25 by agranger          #+#    #+#              #
#    Updated: 2022/07/21 23:47:39 by agranger         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CFLAGS = -Wall -Wextra -Werror -g3 

CC = gcc

SRCS =	ft_itoa.c \
		ft_strlen.c \
		ft_strdup.c \
		ft_strndup.c \
		ft_free.c \
		ft_strcmp.c \
		ft_memset.c \
		ft_calloc.c \
		free_arr_of_str.c \
		ft_strcpy.c \
		ft_strcat.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_split.c

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
