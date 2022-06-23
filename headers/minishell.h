/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 08:46:50 by charoua           #+#    #+#             */
/*   Updated: 2022/06/23 19:37:09 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <signal.h>
# include <stddef.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>
# include <stdbool.h>
# include "../libft/libft.h"

extern int		g_exit_status;

typedef	enum	e_toktype
{
	WORD,
	FD,
	LIMITOR,
	PIPE,
	FILE_IN,
	HEREDOC,
	FILE_OUT,
	FILE_OUT_APP,
	AND,
	OR,
	LPAR,
	RPAR
}	t_toktype;

typedef struct s_pars
{
	char			*str;
	int				token;
	int				db_quote;
	int				sp_quote;
	struct s_pars	*prev;
	struct s_pars	*next;
}	t_pars;

typedef struct s_dblist
{
	t_pars	*first;
	t_pars	*last;
}	t_dblist;

typedef struct	s_node
{
	t_toktype		type;
	char			**cmd;
	struct s_node	*parent;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

typedef struct s_env
{
	char			*var;	//USER
	char			*value;	//marvin42
	char			*full;	//USER=marvin42
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

int	ft_lexer(char *str, t_dblist **list, int *error);
t_pars	*ft_create_pars(t_pars *prev);
int		ft_add_lex(char *str, t_pars **pars, t_dblist **list);
t_pars	*ft_create_pars(t_pars *prev);
int		ft_quote(char *str, char c, t_pars **pars);
int		ft_line(char *str, t_pars **pars);
int		ft_input(char *str, t_pars **pars);
int		ft_output(char *str, t_pars **pars);
int		ft_and(char *str, t_pars **pars);
int		ft_bracket(char c, t_pars **pars, int *bracket);
int		ft_word(char *str, t_pars **pars);
void		ft_check_word(t_dblist **list);
int		ft_syntax_error(t_dblist *list, int bracket);
void		ft_error(int err, t_dblist **list);
void		ft_error_redir(t_dblist **list);
t_node	*ast_create_node(t_toktype type, t_pars **cmd);
void	ast_add_children(t_node *parent, t_node *left_child, t_node *right_child);
void	ast_delete_nodes(t_node *node);
void	ast_add_arg_cmd(t_node **first, t_node *new);
t_node	*cmd_redir1(t_pars **token, int *status);
t_node	*cmd_redir2(t_pars **token, int *status);
t_node	*cmd_redir3(t_pars **token, int *status);
t_node	*cmd(t_pars **token, int *status);
t_node	*cmd1(t_pars **token, int *status);
t_node	*cmd2(t_pars **token, int *status);
t_node	*file(t_pars **token, int *status);
int		parser(t_node **ast, t_pars *token);
void	print_ast(t_node *ast);
void	is_eof(char *input);
void	exit_minishell(void);
void	ft_free_tokens(t_dblist *list);
t_env	*singleton_env(int i, int *status, char **envp);
t_node	*create_cmd(t_pars **first, int *status);
t_node	*create_ast(t_pars **first, bool expr_bracket, int *status);
t_dblist	*create_list(void);

#endif
