/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 08:46:50 by charoua           #+#    #+#             */
/*   Updated: 2022/07/20 00:18:57 by agranger         ###   ########.fr       */
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
# include <dirent.h>
# include <stdio.h>
# include "libft.h"

extern int		g_exit_status;

typedef enum e_toktype
{
	WORD = 0,
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
	RPAR,
	NONE
}	t_toktype;

typedef enum e_nodes
{
	LCMD = 0,
	OP,
	RCMD
}	t_nodes;

typedef enum e_pipe
{
	READ = 0,
	WRITE
}	t_pipe;

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

typedef struct s_node
{
	t_toktype		type;
	char			**cmd;
	int				fd_in;
	int				fd_out;
	struct s_node	*parent;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

typedef struct s_env
{
	char			*var;
	char			*value;
	char			*full;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

void		ft_print_dblist(t_dblist *list);
void		ft_check_word(t_dblist **list);
void		ft_error(int err, t_dblist **list);
void		ft_error_redir(t_dblist **list);
void		ast_delete_nodes(t_node *node);
void		ast_add_arg_cmd(t_node **first, t_node *new);
void		print_ast(t_node *ast);
void		is_eof(char *input);
void		exit_minishell(void);
void		ft_free_tokens(t_dblist *list);
void		free_env(t_env *env);
void		*free_nodes(t_node *redir, t_node *right,
				t_node *left, int *status);
int			ft_lexer(char *str, t_dblist **list, int *error);
int			ft_add_lex(char *str, t_pars **pars, t_dblist **list);
int			ft_quote(char *str, char c, t_pars **pars);
int			ft_line(char *str, t_pars **pars);
int			ft_input(char *str, t_pars **pars);
int			ft_output(char *str, t_pars **pars);
int			ft_and(char *str, t_pars **pars);
int			ft_bracket(char c, t_pars **pars, int *bracket);
int			ft_word(char *str, t_pars **pars);
int			ft_syntax_error(t_dblist *list, int bracket);
int			ft_expand(t_dblist **list, t_env **env);
int			ft_variable(t_pars **exp, t_env *env);
int			ft_wildcard(t_dblist **list, t_pars **exp);
int			ft_copy_pars(t_pars **prev, char *dir);
int			ft_ncmp(char *s1, char *s2, int n);
int			parser(t_node **ast, t_pars *token, int *error);
int			exec(t_node *ast);
char		*ft_replacebyvar(char *str, char *var, int size, int pos);
char		*ft_copy_str(char *dir);
char		**set_status_error(int *status, char **ret);
bool		is_brackets(int type);
bool		is_redir_token(t_pars *token);
bool		is_chevron(int type);
t_pars		*ft_create_pars(t_pars *prev);
t_pars		*ft_create_pars(t_pars *prev);
t_pars		*put_redirs_in_order(t_pars *token);
t_node		*ast_create_node(t_toktype type, t_pars **cmd);
t_node		*ast_add_children(t_node *parent, t_node *left_child,
				t_node *right_child);
t_node		*is_cmd_redir(t_pars **token, int *status);
t_node		*cmd_pipe(t_pars **token, int *status);
t_node		*cmd_redir(t_pars **token, int *status);
t_node		*cmd(t_pars **token, int *status);
t_node		*cmd_brackets(t_pars **token, int *status);
t_node		*cmd_simple(t_pars **token, int *status);
t_node		*file(t_pars **token, int *status);
t_node		*create_cmd(t_pars **first, int *status);
t_node		*create_ast(t_pars **first, bool expr_bracket, int *status);
t_dblist	*create_list(void);
t_env		*singleton_env(int i, int *status, char **envp);

void		vizAST(t_node *ast);

#endif
