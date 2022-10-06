/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 08:46:50 by charoua           #+#    #+#             */
/*   Updated: 2022/10/06 23:51:25 by agranger         ###   ########.fr       */
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
# include <sys/stat.h>
# include <fcntl.h>
# include "libft.h"

# define WRITE_MAX 50000

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

typedef enum e_err
{
	ERR_UNEXPECTED_TOK = 0,
	ERR_MATCHING_TOK,
	ERR_PERM_DENIED,
	ERR_CMD_NOT_FOUND,
	ERR_IS_DIR,
	ERR_NO_FILE,
	ERR_ENV_NBARG,
	ERR_UNSET_ARG,
	ERR_CD_NBARG,
	ERR_EXIT_NBARG,
	ERR_EXIT_ARG_NUM,
	ERR_EXIT_ARG_NO_NUM,
	ERR_AMB_REDIRECT
}	t_err;

typedef enum e_context
{
	INPUT = 0,
	EXEC,
	HDOC,
	IGN
}	t_context;

typedef enum e_sig
{
	INT = 0,
	QUIT,
	SPIPE
}	t_sig;

typedef struct s_pars
{
	char			*str;
	int				token;
	int				db_quote;
	int				sp_quote;
	int				*heredoc;
	struct s_pars	*prev;
	struct s_pars	*next;
}	t_pars;

typedef struct s_dblist
{
	t_pars	*first;
	t_pars	*last;
	t_pars	*curr;
}	t_dblist;

typedef struct s_node
{
	t_toktype		type;
	char			**cmd;
	int				fd_in;
	int				fd_out;
	bool			is_pipe;
	int				*heredoc;
	struct s_node	*parent;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

typedef struct s_env
{
	char			*var;
	char			*value;
	char			*full;
	int				pos;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct sigaction	t_sa;

void		write_pipe_heredoc(int fd, char *str, int len);
void		ft_print_dblist(t_dblist *list);
void		ft_check_word(t_dblist **list);
void		ft_error(int err, t_dblist **list);
void		ft_error_redir(t_dblist **list);
void		ast_delete_nodes(t_node *node);
void		ast_add_arg_cmd(t_node **first, t_node *new);
void		print_ast(t_node *ast);
void		is_eof(char *input);
void		exit_minishell(t_node *ast);
void		ft_free_tokens(t_dblist *list);
void		free_env(t_env *env);
void		ft_env_sort(void);
void		free_tokens_ast(t_node *ast, t_dblist *tokens);
void		ft_free_pars(t_pars *token);
void		*free_nodes(t_node *redir, t_node *right,
				t_node *left, int *status);
void		ft_clear_exp(t_dblist **list, t_pars **exp, int pos);
void		display_error(t_err err, char *arg);
void		remove_pars(t_pars **token);
void		ft_home(t_pars **exp);
void		free_heredoc(t_pars *curr);
void		which_error(t_node *node);
void		exit_child_builtin(t_node *node);
void		close_fd_in_out(t_node *node);
void		clean_heredoc(t_pars *token, t_node *node);
void		mark_chars_to_trim(char *lim);
void		trim_lim(char *lim);
void		check_full_path(t_node *node, char **pathname,
				int *is_dir, int *cmd_not_found);
void		next_logical_node(t_node **node);
void		close_fds_exec_fail(t_node *node);
void		go_to_redir_node(t_node **node, int *in, int *out);
void		move_to_first_cmd(t_node **ast);
void		sigint_input(int signum);
void		sigint_exec(int signum);
void		sigint_hdoc(int signum);
void		sigpipe_hdoc(int signum);
void		sigquit_exec(int signum);
void		add_back_env(t_env **first, t_env *new);
int 		fork_heredoc_child(t_dblist **list, t_sa *sig, int *pipe_heredoc, char *lim);
int			fork_heredoc_parent(pid_t pid, t_pars *token, int *pipe_heredoc);
int			convert_status(int status);
int			is_eof_heredoc(char *input, char *lim, int line);
int			set_signal(t_context context, t_sa *sa);
int			word_splitting(t_pars **exp, bool dquote);
int			init_signals(void);
int			exec_bin(t_node *node, t_sa *sig);
int			dup2_if_needed(t_node *node);
int			exec_cmd_fork(t_node *node, pid_t pid, t_sa *sig);
int			init_fd(t_node *node, int *pipe_fd);
int			init_pipe(t_node *node, int prev_fd, int *pipe_fd, int ret);
int			check_file_in_out(t_node *node);
int			set_file_in(t_node *node, t_node *cmd);
int			set_file_out(t_node *node, t_node *cmd);
int			file_in_exist(t_node *node, t_node *cmd);
int			create_file_out(t_node *node, t_node *cmd);
int			create_file_out_app(t_node *node, t_node *cmd);
int			set_heredoc(t_node *node, t_node *cmd);
int			launch_heredoc(t_dblist **list, int *pipe_heredoc, char *lim, t_sa *sig);
int			ft_check_wildcard(t_pars **exp, t_dblist **list);
int			check_is_heredoc(char *lim, t_sa *sig, t_dblist **list);
int			ft_exp_quote(t_pars **exp, int i, char c, int *error);
int			ft_lexer(char *str, t_dblist **list, int *error, t_sa *sig);
int			ft_add_lex(char *str, t_pars **pars, t_dblist **list, t_sa *sig);
int			ft_quote(char *str);
int			ft_line(char *str, t_pars *pars);
int			ft_input(char *str, t_pars *pars);
int			ft_output(char *str, t_pars *pars);
int			ft_and(char *str, t_pars *pars);
int			ft_bracket(char c, t_pars *pars, int *bracket);
int			ft_word(char *str, t_dblist **list, t_sa *sig);
int			ft_syntax_error(t_dblist *list, int bracket);
int			ft_expand(t_dblist **list, int *error);
int			ft_variable(t_pars **exp, int *j, int *error, bool dquote);
int			ft_wildcard(t_dblist **list, t_pars **exp);
int			ft_copy_pars(t_pars **prev, char *dir);
int			ft_ncmp(char *s1, char *s2, int n);
int			parser(t_node **ast, t_pars *token, int *error);
int			exec(t_node *ast, t_sa *sig);
int			ft_cd(t_node *node);
int			ft_echo(t_node *node);
int			ft_env(t_node *node);
int			ft_exit(t_node *node);
int			ft_export(t_node *node);
int			ft_pwd(t_node *node);
int			ft_unset(t_node *node);
int			look_for_heredocs(t_pars *token);
int			check_syntax(t_pars *token, char *str, int index, int bracket);
int			init_shlvl(t_env *shell);
int			no_env_or_cmd_empty(int *cmd_not_found, char **paths);
int			find_path_bin(t_node *node, char **pathname, int *cmd_not_found);
int			get_status_last_process(pid_t *pids);
int			exec_builtin(t_node *ast, int (*ft_builtin)(t_node *node));
int			exec_cmd_wo_fork(t_node *node);
char		*heredoc_expansion(char *str);
char		*replace_exp_heredoc(char *str, int start, int len, char *var);
char		*get_env_var_heredoc(char *var);
char		*ft_replacebyvar(char *str, char *var, int size, int pos);
char		*ft_copy_str(char *dir);
char		*concat_pathname(char *path, char *cmd);
char		**set_status_error(int *status, char **ret);
char		**env_to_str_arr(t_env *env);
char		**get_envpath_value(void);
bool		is_brackets(int type);
bool		is_redir_token(t_pars *token);
bool		is_chevron(int type);
bool		must_be_appended(t_node *node);
bool		check_error(t_node *node, int is_dir, int cmd_not_found);
bool		must_be_expanded(char *lim);
bool		contain_slash(char *cmd);
bool		check_logical_node(t_node **node, pid_t *pids);
bool		check_status(t_node **node, int status);
bool		cmd_is(char *cmd, char *builtin);
bool		is_builtin_no_fork(char *cmd);
bool		is_uniq_cmd(t_node *node);
bool		is_last_cmd(t_node *node);
bool		is_first_cmd(t_node *node);
bool		is_pipe_cmd(t_node *node);
bool		check_ambiguous_redirect(t_pars **exp, t_env *env, char *str, int *error);
t_pars		*ft_create_pars(t_pars *prev);
t_pars		*ft_create_pars(t_pars *prev);
t_pars		*put_redirs_in_order(t_pars *token);
t_node		*next_cmd(t_node *node);
t_node		*next_cmd_after_redir(t_node *node);
t_node		*ast_create_node(t_toktype type, t_pars **cmd);
t_node		*ast_add_children(t_node *parent, t_node *left_child,
				t_node *right_child);
t_node		*next_cmd_logical_node(t_node *node);
t_node		*cmd_logical_operator(t_pars **token, int *status);
t_node		*is_cmd_redir(t_pars **token, int *status);
t_node		*is_cmd_pipe(t_pars **token, int *status);
t_node		*cmd_pipe(t_pars **token, int *status);
t_node		*cmd_redir(t_pars **token, int *status);
t_node		*cmd(t_pars **token, int *status);
t_node		*cmd_brackets(t_pars **token, int *status);
t_node		*cmd_simple(t_pars **token, int *status);
t_node		*file(t_pars **token, int *status);
t_node		*create_cmd(t_pars **first, int *status);
t_node		*create_ast(t_pars **first, bool expr_bracket, int *status);
t_node		*put_next_node_above(t_node *ret, int *status, t_pars **token,
				t_node *(*cmd_next_node)(t_pars **token, int *status));
t_dblist	*create_list(void);
t_env		*singleton_env(int i, int *status, char **envp);
t_env		*new_env(char *str);
pid_t		*init_pid_arr(t_node *cmd, int *index_cmd);

void		vizAST(t_node *ast);

#endif
