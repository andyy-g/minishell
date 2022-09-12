/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:19:50 by agranger          #+#    #+#             */
/*   Updated: 2022/09/12 14:37:25 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/********* TOOLS ********/

bool	cmd_is(char *cmd, char *builtin)
{
	if (!ft_strcmp(cmd, builtin))
		return (true);
	return (false);
}

bool	is_builtin_no_fork(char *cmd)
{
	if (cmd_is(cmd, "cd")
			|| cmd_is(cmd, "unset")
			|| cmd_is(cmd, "exit"))
		return (true);
	if (cmd_is(cmd, "export") && !cmd[1])
		return (true);
	return (false);
}

bool	is_uniq_cmd(t_node *node)
{
	while (node->parent)
		node = node->parent;
	if (is_chevron(node->type) || node->type == WORD)
		return (true);
	return (false);
}

bool	is_pipe_cmd(t_node *node)
{
	while (node->parent)
	{
		node = node->parent;
		if (node->type == PIPE)
			return (true);
	}
	return (false);
}

t_node	*next_cmd(t_node *node)
{
	t_node	*prev;

	prev = NULL;
	while (node && node->type != PIPE
			&& node->type != AND && node->type != OR)
	{
		prev = node;
		node = node->parent;
	}
	while (node && prev == node->right)
	{
		prev = node;
		node = node->parent;
	}
	if (node && node->type != PIPE)
		return (NULL);
	if (node)
	{
		node = node->right;
		while (node->left)
			node = node->left;
	}
	return (node);
}

t_node	*next_cmd_after_redir(t_node *node)
{
	t_node	*prev;

	prev = NULL;
	while (node && node->type != FILE_IN)
		node = node->parent;
	while (node && node->type != PIPE
			&& node->type != AND && node->type != OR)
	{
		prev = node;
		node = node->parent;
	}
	while (node && prev == node->right)
	{
		prev = node;
		node = node->parent;
	}
	if (node && node->type != PIPE)
		return (NULL);
	if (node)
	{
		node = node->right;
		while (node->left)
			node = node->left;
	}
	return (node);
}

bool	is_first_cmd(t_node *node)
{
	t_node	*prev;

	while (node->parent && is_chevron(node->parent->type))
		node = node->parent;
	while (node->parent && node->parent->type == PIPE)
	{
		prev = node;
		node = node->parent;
		if (prev != node->left)
			return (false);
	}
	return (true);
}

bool	is_last_cmd(t_node *node)
{
	t_node	*prev;

	while (node->parent && is_chevron(node->parent->type))
		node = node->parent;
	while (node->parent && node->parent->type == PIPE)
	{
		prev = node;
		node = node->parent;
		if (prev != node->right)
			return (false);
	}
	return (true);
}

/************************/

int	get_status_last_process(pid_t *pids)
{
	int	i;
	int	status;

	i = 0;
	while (pids[i] != -1)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
	return (status);
}

void	next_logical_node(t_node **node)
{
	t_node	*prev;

	if (((*node)->type == AND || (*node)->type == OR))
	{
		*node = (*node)->parent;
		if (*node && ((*node)->type == AND || (*node)->type == OR))
			return ;
	}
	prev = NULL;
	while (*node && ((*node)->type == WORD || is_chevron((*node)->type)))
	{
		prev = *node;
		*node = (*node)->parent;
	}
	if (!*node || prev == (*node)->left)
		return ;
	while ((*node)->parent &&
			((*node)->parent->right == *node || is_chevron((*node)->parent->type)))
		*node = (*node)->parent;
	*node = (*node)->parent;
}

t_node	*next_cmd_logical_node(t_node *node)
{
	node = node->right;
	while (node->left)
		node = node->left;
	return (node);
}

bool	check_status(t_node **node, int status)
{
	while (*node)
	{
		if ((*node)->type == AND)
		{
			if (status == 0)
			{
				*node = next_cmd_logical_node(*node);
				return (true);
			}
			next_logical_node(node);
		}
		else if ((*node)->type == OR)
		{
			if (status != 0)
			{
				*node = next_cmd_logical_node(*node);
				return (true);
			}
			next_logical_node(node);
		}
	}
	return (true);
}

bool	check_logical_node(t_node **node, pid_t *pids)
{
	int		status;

	next_logical_node(node);
	if (!*node || (*node)->type == PIPE)
		return (false);
	status = get_status_last_process(pids);
	return (check_status(node, status));
}

char	**get_envpath_value(void)
{
	t_env	*env;
	char	**paths;

	env = singleton_env(1, NULL, NULL);
	while (env && ft_strcmp(env->var, "PATH"))
		env = env->next;
	if (!env)
		return (NULL);
	paths = ft_split(env->value, ':');
	return (paths);
}

char	*concat_pathname(char *path, char *cmd)
{
	int		size;
	char	*pathname;

	size = ft_strlen(path) + ft_strlen(cmd) + 1;
	pathname = ft_calloc((size + 1), sizeof(char));
	ft_strcpy(pathname, path);
	ft_strcat(pathname, "/");
	ft_strcat(pathname, cmd);
	return (pathname);
}

int	find_path_bin(t_node *node, char **pathname, int *cmd_not_found)
{
	char		**paths;
	struct stat	sb;
	int			i;

	sb.st_mode = 0;
	paths = get_envpath_value();
	if (!paths || !node->cmd[0][0])
	{
		*cmd_not_found = 1;
		return (1);
	}
	i = 0;
	while (paths[i])
	{
		*pathname = concat_pathname(paths[i], node->cmd[0]);
		if (!*pathname)
		{
			free_arr_of_str(paths);
			return (0);
		}
		if ((stat(*pathname, &sb) == 0 && S_ISDIR(sb.st_mode) == 0))
			break ;
		ft_free(*pathname);
		i++;
	}
	if (!paths[i])
		*cmd_not_found = 1;
	free_arr_of_str(paths);
	return (1);
}

bool	contain_slash(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (true);
		i++;
	}
	return (false);
}

void	check_relative_absolute_path(t_node *node, char **pathname, int *is_dir, int *cmd_not_found)
{
	struct stat	sb;
	char		**paths;

	paths = get_envpath_value();
	sb.st_mode = 0;
	stat(node->cmd[0], &sb);
	if (S_ISDIR(sb.st_mode))
	{
		*is_dir = 1;
		return ;
	}
	if (paths && !contain_slash(node->cmd[0]))
	{
		*cmd_not_found = 1;
		return ;
	}
	*cmd_not_found = 0;
	*pathname = ft_strdup(node->cmd[0]);
}

void	trim_quotes(char *lim, int *i, char quote, int *len)
{
	int	add;
	*len -= 2;
	add = 1;
	if (*i > 0 && lim[*i - 1] == '$')
	{
		add++;
		(*i)--;
	}
	while (*i < *len && lim[*i + add])
	{
		while (lim[*i + add] && (lim[*i + add] == quote || lim[*i + add] == '$'))
			add++;
		lim[*i] = lim[*i + add];
		if (lim[*i + add])
			(*i)++;
	}
	lim[*i] = '\0';
}

bool	must_be_expanded(char *lim)
{
	int		i;
	char	quote;
	int		len;

	i = 0;
	quote = 0;
	len = ft_strlen(lim);
	while (lim[i])
	{
		if (lim[i] == '\'' || lim[i] == '"')
		{
			quote = lim[i];
			trim_quotes(lim, &i, quote, &len);
			break ;
		}
		i++;
	}
	if (quote)
		return (false);
	return (true);
}

char	*get_env_var_heredoc(char *var)
{
	t_env	*env;

	env = singleton_env(1, NULL, NULL);
	while (env && ft_strcmp(env->var, var))
		env = env->next;
	if (!env)
		return (NULL);
	return (env->value);
}

char	*replace_exp_heredoc(char *str, int start, int len, char *var)
{
	char	*ret;
	int		size;
	int		i;

	size = ft_strlen(str) - len + ft_strlen(var);
	ret = malloc(sizeof(char) * (size + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (str[i] && i < start)
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	ft_strcat(ret, var);
	ft_strcat(ret, &str[i + len]);
	return (ret);
}

char	*heredoc_expansion(char *str)
{
	int		pos;
	int		size;
	char	*ret;
	char	*var;
	char	*value;

	pos = 0;
	while (str[pos] && str[pos] != '$')
		pos++;
	if (!str[pos])
		return (ft_strdup(str));
	size = 0;
	pos++;
	while (str[pos + size] && (ft_isalnum(str[pos + size]) || str[pos + size] == '_'))
		size++;
	var = ft_substr(str, pos, size);
	if (!var)
		return (NULL);
	value = get_env_var_heredoc(var);
	if (!value)
		return (ft_strdup(str));
	ret = replace_exp_heredoc(str, pos - 1, size + 1, value);
	return (ret);
}

int	launch_heredoc(t_pars *token, int *pipe_heredoc, char *lim)
{
	char	*input;
	bool	expansion;

	input = NULL;
	expansion = must_be_expanded(lim);
	while (1)
	{
		input = readline("> ");
		if (!ft_strcmp(lim, input))
			break ;
		if (expansion)
		{
			input = heredoc_expansion(input);
			if (!input)
				return (0);
			write(pipe_heredoc[WRITE], input, ft_strlen(input));
			ft_free(input);
		}
		else
			write(pipe_heredoc[WRITE], input, ft_strlen(input));
		write(pipe_heredoc[WRITE], "\n", 1);
	}
	if (token->heredoc)
	{
		close(token->heredoc[READ]);
		ft_free(token->heredoc);
	}
	token->heredoc = pipe_heredoc;
	close(pipe_heredoc[WRITE]);
	return (1);
}

int	check_is_heredoc(t_pars *token, char *lim)
{
	int	*pipe_heredoc;
	int	ret;

	ret = 1;
	if (!lim)
		return (0);
	if (token && token->str && token->token == HEREDOC)
	{
		pipe_heredoc = malloc(sizeof(int) * 2);
		if (pipe(pipe_heredoc) == -1)
		{
			perror("pipe");
			ft_free(lim);
			return (0);
		}
		if (!launch_heredoc(token, pipe_heredoc, lim))
			ret = 0;
	}
	ft_free(lim);
	return (ret);
}

int	exec_builtin(t_node *ast, int (*ft_builtin)(t_node *node))
{
	int	ret;

	ret = ft_builtin(ast);
	return (ret);
}

void	close_fds_exec_fail(t_node *node)
{
	if (node->parent && node->parent->heredoc)
	{   
		close(node->parent->heredoc[0]);
		ft_free(node->parent->heredoc);
	} 
	if (node->fd_in != STDIN_FILENO)
	{
		close(node->fd_in);
		close(STDIN_FILENO);
	}
	if (node->fd_out != STDOUT_FILENO)
	{
		close(node->fd_out);
		close(STDOUT_FILENO);
	}
}

bool	check_error(t_node *node, int is_dir, int cmd_not_found)
{
	if (is_dir)
	{
		display_error(ERR_IS_DIR, node->cmd[0]);
		return (false);
	}
	if (cmd_not_found)
	{
		display_error(ERR_CMD_NOT_FOUND, node->cmd[0]);
		return (false);
	}
	return (true);
}

int	exec_bin(t_node *node)
{
	char	**envp;
	char	*path;	
	int		is_dir;
	int		cmd_not_found;

	is_dir = 0;
	cmd_not_found = 0;
	if (is_chevron(node->type))
		return (1);
	path = NULL;
	envp = env_to_str_arr(singleton_env(1, NULL, NULL));
	if (!envp || !find_path_bin(node, &path, &cmd_not_found))
		return (0);
	if (cmd_not_found)
		check_relative_absolute_path(node, &path, &is_dir, &cmd_not_found);
	if (check_error(node, is_dir, cmd_not_found))
	{
		execve(path, node->cmd, envp);
		if (errno == EACCES)
			display_error(ERR_PERM_DENIED, node->cmd[0]);
		if (errno == ENOENT)
			display_error(ERR_NO_FILE, node->cmd[0]);
		ft_free(path);
	}
	ft_free(envp);
	close_fds_exec_fail(node);
	exit_minishell(node);
	return (1);
}

int	file_in_exist(t_node *node, t_node *cmd)
{
	int	fd;

	if (access(node->right->cmd[0], F_OK) == -1)
	{
		display_error(ERR_NO_FILE, node->right->cmd[0]);
		return (2);
	}
	fd = open(node->right->cmd[0], O_RDONLY | O_CLOEXEC);
	if (fd == -1)
	{
		perror("open");
		return (0);
	}
	if (cmd && cmd->fd_in != 0)
		close(cmd->fd_in);
	if (cmd)
		cmd->fd_in = fd;
	return (1);
}

bool	must_be_appended(t_node *node)
{
	t_node	*prev;

	prev = NULL;
	while (node && node->type != AND && node->type != OR)
	{
		prev = node;
		node = node->parent;
	}
	if (node && node->right == prev)
		return (true);
	return (false);
}

int	create_file_out(t_node *node, t_node *cmd)
{
	int	fd;

	if (must_be_appended(cmd)) 
		fd = open(node->right->cmd[0], O_WRONLY | O_CREAT
				| O_APPEND | O_CLOEXEC, 0644);
	else
		fd = open(node->right->cmd[0], O_WRONLY | O_CREAT
				| O_TRUNC | O_CLOEXEC, 0644);
	if (fd == -1)
	{
		perror("open");
		return (0);
	}
	if (cmd && cmd->fd_out != 1)
		close(cmd->fd_out);
	if (cmd)
		cmd->fd_out = fd;
	return (1);
}

int	create_file_out_app(t_node *node, t_node *cmd)
{
	int	fd;

	fd = open(node->right->cmd[0], O_WRONLY | O_CREAT
			| O_APPEND | O_CLOEXEC, 0644);
	if (fd == -1)
	{
		perror("open");
		return (0);
	}
	if (cmd && cmd->fd_out != 1)
		close(cmd->fd_out);
	if (cmd)
		cmd->fd_out = fd;
	return (1);
}

int	fd_heredoc(t_node *node, t_node *cmd)
{
	if (cmd && node->heredoc)
		cmd->fd_in = node->heredoc[READ];
	return (1);
}

int	check_file_out(t_node *node)
{
	t_node	*cmd;
	t_node	*prev;

	cmd = node;
	prev = NULL;
	while (node && !is_chevron(node->type))
	{
		if (node->type == PIPE && prev == node->left)
			break ;
		prev = node;
		node = node->parent;
	}
	while (node && is_chevron(node->type))
	{
		if (node->type == FILE_OUT)
			if (!create_file_out(node, cmd))
				return (0);
		if (node->type == FILE_OUT_APP)
			if (!create_file_out_app(node, cmd))
				return (0);
		node = node->parent;
	}
	return (1);
}
int	check_file_in(t_node *node)
{
	t_node	*cmd;
	t_node	*prev;
	int		ret;

	cmd = node;
	prev = NULL;
	while (node && !is_chevron(node->type))
	{
		if (node->type == PIPE && prev == node->right)
			break ;
		prev = node;
		node = node->parent;
	}
	while (node && is_chevron(node->type))
	{
		if (node->type == FILE_IN)
		{
			ret = file_in_exist(node, cmd);
			if (!ret || ret == 2)
				return (ret);
		}
		if (node->type == HEREDOC)
			if (!fd_heredoc(node, cmd))
				return (0);
		node = node->parent;
	}
	return (1);
}

int	exec_cmd_wo_fork(t_node *node)
{
	int	ret;

	ret = 1;
	if (cmd_is(node->cmd[0], "cd"))
		printf("CD\n");
	//ret = ft_cd;
	else if (cmd_is(node->cmd[0], "export"))
		printf("EXPORT\n");
	//ret = ft_export;
	else if (cmd_is(node->cmd[0], "unset"))
		printf("UNSET\n");
	//ret = ft_unset;
	else if (cmd_is(node->cmd[0], "exit"))
		printf("EXIT\n");
	//ret = ft_exit;
	if (node->fd_in != STDIN_FILENO)
		close(node->fd_in);
	if (node->fd_out != STDOUT_FILENO)
		close(node->fd_out);
	return (ret);
}

void	exit_child_builtin(t_node *node)
{
	if (node->fd_in != STDIN_FILENO)
	{
		close(node->fd_in);
		close(STDIN_FILENO);
	}
	if (node->fd_out != STDOUT_FILENO)
	{
		close(node->fd_out);
		close(STDOUT_FILENO);
	}
	while (node->parent)
		node = node->parent;
	exit_minishell(node);
}

int	exec_cmd_fork(t_node *node, pid_t pid)
{
	int	ret;

	if (node->fd_in != STDIN_FILENO)
		if (dup2(node->fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (0);
		}
	if (node->fd_out != STDOUT_FILENO)
		if (dup2(node->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			return(0);
		}
	ret = 1;
	if (cmd_is(node->cmd[0], "echo"))
		ret = exec_builtin(node, &ft_echo);
	else if (cmd_is(node->cmd[0], "pwd"))
		printf("PWD\n");
	//ret = exec_builtin(node, &ft_pwd, pid);
	else if (cmd_is(node->cmd[0], "export"))
		printf("EXPORT\n");
	//ret = exec_builtin(node, &ft_export, pid);
	else if (cmd_is(node->cmd[0], "env"))
		printf("ENV\n");
	//ret = exec_builtin(node, &ft_env);
	else if (cmd_is(node->cmd[0], "cd"))
		printf("CD\n");
	//ret = ft_cd;
	else if (cmd_is(node->cmd[0], "unset"))
		printf("UNSET\n");
	//ret = ft_unset;
	else if (cmd_is(node->cmd[0], "exit"))
		printf("EXIT\n");
	//ret = ft_exit;

	else
		ret = exec_bin(node);
	if (pid == 0)
		exit_child_builtin(node);
	return (ret);
}

int	init_fd(t_node *node, int *pipe_fd)
{
	int	in;
	int	out;
	int	prev_fd;

	if (pipe_fd)
		prev_fd = pipe_fd[READ];
	in = check_file_in(node);
	out = check_file_out(node);
	if (!in || !out)
		return (0);
	if (!is_pipe_cmd(node))
	{
		if (in == 2 || out == 2)
			return (2);
		return (1);
	}
	node->is_pipe = true;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	if (in == 2 || out == 2)
		return (2);
	if (!is_first_cmd(node) && node->fd_in == STDIN_FILENO)
		node->fd_in = prev_fd;
	else if (prev_fd != -1)
		close(prev_fd);
	if (!is_last_cmd(node) && node->fd_out == STDOUT_FILENO)
		node->fd_out = pipe_fd[WRITE];
	else
		close(pipe_fd[WRITE]);
	return (1);
}

int	fork_process(t_node *ast, int *pipe_fd, pid_t **pids, int index_cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (0);
	}
	if (pid == 0)
	{
		free(*pids);
		if (ast->is_pipe)
			close(pipe_fd[READ]);
		if (exec_cmd_fork(ast, pid))
			return (0);
	}
	if (ast->is_pipe)
		close(pipe_fd[WRITE]);
	if (ast->fd_in != 0)
		close(ast->fd_in);
	if (ast->fd_out != 1)
		close(ast->fd_out);
	(*pids)[index_cmd] = pid;
	return (1);
}

pid_t	*init_pid_arr(t_node *cmd, int *index_cmd)
{
	int		nb_cmd;
	pid_t	*pids;

	nb_cmd = 0;
	while (cmd)
	{
		nb_cmd++;
		cmd = next_cmd(cmd);
	}
	pids = calloc(nb_cmd + 1, sizeof(*pids));
	pids[nb_cmd] = -1;
	*index_cmd = 0;
	return (pids);
}

int	tree_traversal(t_node *cmd, int *pipe_fd, pid_t **pids, int index_cmd)
{
	int	ret;

	while (cmd)
	{
		ret = init_fd(cmd, pipe_fd);
		if (ret == 2)
		{
			if (cmd->is_pipe)
				close(pipe_fd[WRITE]);
			cmd = next_cmd_after_redir(cmd);
			continue ;
		}
		if (!ret || !fork_process(cmd, pipe_fd, pids, index_cmd))
			return (0);
		if (cmd->parent && cmd->parent->heredoc)
		{   
			close(cmd->parent->heredoc[0]);
			ft_free(cmd->parent->heredoc);
		}   
		if (!check_logical_node(&cmd, *pids))
		{
			cmd = next_cmd(cmd);
			index_cmd++;
		}
		else
		{
			ft_free(*pids);
			*pids = init_pid_arr(cmd, &index_cmd);
		}
	}
	close(pipe_fd[READ]);
	return (1);
}

int	launch_exec_fork(t_node *cmd)
{
	int		status;
	int		index_cmd;
	int		pipe_fd[2];
	pid_t	*pids;

	pipe_fd[READ] = -1;
	pipe_fd[WRITE] = -1;
	pids = init_pid_arr(cmd, &index_cmd);
	if (!pids)
		return (0);
	if (!tree_traversal(cmd, pipe_fd, &pids, index_cmd))
		return (0);
	status = get_status_last_process(pids);
	(void)status;
	ft_free(pids);
	return (1);
}

void	move_to_first_cmd(t_node **ast)
{
	while ((*ast)->left)
		*ast = (*ast)->left;
}

int	exec(t_node *ast)
{
	int	ret;

	move_to_first_cmd(&ast);
	if (is_builtin_no_fork(ast->cmd[0]) && is_uniq_cmd(ast))
	{	
		ret = init_fd(ast, NULL);
		if (ret == 2)
			return (1);
		if (!ret || !exec_cmd_wo_fork(ast))
			return (0);
		return (1);
	}
	if (!launch_exec_fork(ast))
		return (0);
	return (1);
}
