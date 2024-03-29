# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tcarasso <tcarasso@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/11 14:47:55 by tcarasso          #+#    #+#              #
#    Updated: 2022/10/09 07:25:51 by agranger         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	minishell

CFLAGS			=	-Wall -Wextra -Werror -g3

CC				=	gcc

SRCDIR			=	./srcs

OBJDIR			=	./objs

HEADERDIR		=	./headers

LIBFTDIR		=	./libft

LIBFT			=	$(LIBFTDIR)/libft.a

LIBRARY			=	-lreadline -lft -L$(LIBFTDIR)

CFILES			=	main.c \
					lexer/ft_lexer.c \
					lexer/ft_manage_struct.c \
					lexer/ft_token_bis.c \
					lexer/ft_token.c \
					error/ft_syntax_error.c \
					error/ft_display_error.c \
					error/ft_display_error_bis.c \
					error/ambiguous_redirect.c \
					expansion/ft_expand.c \
					expansion/ft_expand_specials_chars.c \
					expansion/ft_variable.c \
					expansion/ft_variable_utils.c \
					expansion/ft_wildcard.c \
					expansion/ft_wildcard_cpy.c \
					expansion/word_splitting.c \
					parser/node.c \
					parser/parser.c \
					parser/cmd_w_operator.c \
					parser/cmd_w_redir.c \
					parser/cmd_simple.c \
					parser/sort_redir.c \
					parser/create_str_cmd.c \
					parser/tools.c \
					exit/exit.c \
					signals/eof.c \
					signals/set_signals.c \
					signals/handler.c \
					signals/signals_exec.c \
					env/singleton_env.c \
					env/sort_env.c \
					env/free_env.c \
					env/conversion_env.c \
					env/shlvl.c \
					exec/exec.c \
					exec/init_fd_cmd.c \
					exec/check_full_path.c \
					exec/clean.c \
					exec/create_file_in_out.c \
					exec/error_exec.c \
					exec/exec_w_fork.c \
					exec/exec_wo_fork.c \
					exec/find_absolute_path.c \
					exec/heredoc_tools.c \
					exec/launch_heredoc.c \
					exec/logical_node.c \
					exec/fork_heredoc.c \
					exec/move_redir.c \
					exec/tools.c \
					builtin/ft_cd.c \
					builtin/ft_cd_utils.c \
					builtin/ft_echo.c \
					builtin/ft_env.c \
					builtin/ft_exit.c \
					builtin/ft_export.c \
					builtin/ft_export_utils.c \
					builtin/ft_export_utils_bis.c \
					builtin/ft_pwd.c \
					builtin/ft_unset.c
					
HFILES			=	$(HEADERDIR)/minishell.h

SRCS			=	$(addprefix $(SRCDIR)/,$(CFILES))

OBJS			=	$(addprefix $(OBJDIR)/,$(CFILES:.c=.o))

RM 				=	rm -rf


all				:	$(NAME)

$(NAME)			:	$(LIBFT) $(OBJDIR) $(OBJS) $(HFILES)
					@$(CC) $(CFLAGS) -I./$(HEADERDIR) $(OBJS) $(LIBRARY) -o $(NAME) 
					@echo "$(NAME) created"

$(LIBFT)		:
					@make -sC $(LIBFTDIR)

$(OBJDIR)		:
					@mkdir -p $(OBJDIR) \
						$(OBJDIR)/lexer \
						$(OBJDIR)/parser \
						$(OBJDIR)/exit \
						$(OBJDIR)/expansion \
						$(OBJDIR)/signals \
						$(OBJDIR)/env \
						$(OBJDIR)/error \
						$(OBJDIR)/exec \
						$(OBJDIR)/builtin

$(OBJDIR)/%.o	:	$(SRCDIR)/%.c
					@$(CC) -o $@ -c $< $(CFLAGS) -I./$(HEADERDIR)

clean			: 
					@$(RM) $(OBJDIR)
					@make clean -sC $(LIBFTDIR)
					@echo "object files $(NAME) removed"

fclean			:	clean
					@make fclean -sC $(LIBFTDIR)
					@$(RM) $(NAME) 
					@echo "$(NAME) and libft.a removed"

re				:	fclean all

malloc_test		:	$(LIBFT) $(OBJDIR) $(OBJS) $(HFILES)
					$(CC) $(CFLAGS) -fsanitize=undefined -rdynamic -o $@ $(OBJS) $(LIBRARY) -ldl -L. -lmallocator

vizast			:	$(LIBFT) $(OBJDIR) $(OBJS) $(HFILES)
					@$(CC) $(CFLAGS) -I./$(HEADERDIR) $(OBJS) $(LIBRARY) -L. -lvizast -o $@

.PHONY			:	clean fclean re all
