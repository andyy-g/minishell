/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:19:50 by agranger          #+#    #+#             */
/*   Updated: 2022/08/24 15:41:35 by agranger         ###   ########.fr       */
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

bool	is_first_cmd(t_node *node)
{
	t_node	*prev;

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

void	next_logical_node(t_node **node)
{
	t_node	*prev;
	if (((*node)->type == AND || (*node)->type == OR))
	{
		*node = (*node)->parent;
		return ;
	}
	prev = NULL;
	while (*node && ((*node)->type == WORD || is_chevron((*node)->type)))
	{
		prev = *node;
		*node = (*node)->parent;
	}
	if (prev == (*node)->left)
		return ;
	while ((*node)->parent && (*node)->parent->right == *node)
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
	int		i;

	next_logical_node(node);
	if (!*node || (*node)->type == PIPE)
		return (false);
	i = 0;
	while (pids[i] != -1)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
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

int	find_path_bin(t_node *node, char **pathname)
{
	char		**paths;
	struct stat	sb;
	int			i;

	paths = get_envpath_value();
	if (!paths)
		return (1);
	i = 0;
	while (paths[i] && (!*pathname || stat(*pathname, &sb) == -1))
	{
		if (*pathname)
			free(*pathname);
		*pathname = concat_pathname(paths[i], node->cmd[0]);
		if (!*pathname)
		{
			free_arr_of_str(paths);
			return (0);
		}
		i++;
	}
	free_arr_of_str(paths);
	return (1);
}

int	look_for_heredocs(t_node *ast)
{
	(void)ast;
	return (1);
}

int	exec_builtin(t_node *ast, int (*ft_builtin)(t_node *node))
{
	int	ret;

	ret = ft_builtin(ast);
	return (ret);
}

int	exec_bin(t_node *node)
{
	char	**env;
	char	*path;	

	path = NULL;
	env = env_to_str_arr(singleton_env(1, NULL, NULL));
	if (!env)
		return (0);
	if (!find_path_bin(node, &path))
		return (0);
	execve(path, node->cmd, env);
	if (errno == ENOENT)
		printf("%s: command not found\n", path);
	//exit_value
	free(env);
	ft_free(path);
	while (node->parent)
		node = node->parent;
	exit_minishell(node);
	return (1);
}

int	file_in_exist(t_node *node, t_node *cmd)
{
	int	fd;
	if (access(node->right->cmd[0], F_OK) == -1)
	{
		printf("bash: %s: No such file or directory\n", node->right->cmd[0]);
		return (2);
	}
	fd = open(node->right->cmd[0], O_RDONLY | O_CLOEXEC);
	if (fd == -1)
	{
		perror("open");
		return (0);
	}
	if (cmd->fd_in != 0)
		close(cmd->fd_in);
	cmd->fd_in = fd;
	return (1);
}

int	create_file_out(t_node *node, t_node *cmd)
{
	int fd;

	fd = open(node->right->cmd[0], O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
	if (fd == -1)
	{
		perror("open");
		return (0);
	}
	if (cmd->fd_out != 1)
		close(cmd->fd_out);
	cmd->fd_out = fd;
	return (1);
}

int	create_file_out_app(t_node *node, t_node *cmd)
{
	int	fd;

	fd = open(node->right->cmd[0], O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644);
	if (fd == -1)
	{
		perror("open");
		return (0);
	}
	if (cmd->fd_out != 1)
		close(cmd->fd_out);
	cmd->fd_out = fd;
	return (1);
}

int	check_file_in_out(t_node *node)
{
	t_node	*cmd;

	cmd = node;
	node = node->parent;
	while (node && is_chevron(node->type))
	{
		if (node->type == FILE_IN)
			if (!file_in_exist(node, cmd))
				return (0);
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

int	exec_cmd_wo_fork(t_node *node)
{
	int ret;

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
		dup2(node->fd_in, STDIN_FILENO);
	if (node->fd_out != STDOUT_FILENO)
		dup2(node->fd_out, STDOUT_FILENO);
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
	else
		ret = exec_bin(node);
	if (pid == 0)
		exit_child_builtin(node);
	return (ret);
}

int	init_fd(t_node *node, int *pipe_fd)
{
	int	ret;
	int	prev_fd;

	ret = check_file_in_out(node);
	prev_fd = pipe_fd[READ];
	if (ret == 0 || ret == 2)
		return (ret);
	if (!is_pipe_cmd(node))		
		return (1);
	node->is_pipe = true;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
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
		if (!exec_cmd_fork(ast, pid))
			return (0);
	}
	else
	{
		if (ast->is_pipe)
			close(pipe_fd[WRITE]);
		if (ast->fd_in != 0)
			close(ast->fd_in);
		if (ast->fd_out != 1)
			close(ast->fd_out);
		(*pids)[index_cmd] = pid;
	}
	return (1);
}

pid_t	*init_pid_arr(t_node *cmd)
{
	int		nb_cmd;
	pid_t	*pids;

	nb_cmd = 0;
	while (cmd)
	{
		nb_cmd++;
		cmd = next_cmd(cmd);
	}
	pids = malloc(sizeof(*pids) * (nb_cmd + 1));
	pids[nb_cmd] = -1;
	return (pids);
}

int	tree_traversal(t_node *cmd)
{
	int		ret;
	int		status;
	int		pipe_fd[2];
	pid_t	*pids;
	int		index_cmd;
	int		i;

	pipe_fd[READ] = -1;
	pipe_fd[WRITE] = -1;
	pids = init_pid_arr(cmd);
	if (!pids)
		return (0);
	index_cmd = 0;
	while (cmd)
	{
		ret = init_fd(cmd, pipe_fd);
		if (ret == 2)
		{
			cmd = next_cmd(cmd);
			continue ;
		}
		if (!ret || !fork_process(cmd, pipe_fd, &pids, index_cmd))
			return (0);
		if (!check_logical_node(&cmd, pids))
		{
			cmd = next_cmd(cmd);
			index_cmd++;
		}
		else
		{
			index_cmd = 0;
			free(pids);
			pids = NULL;
			pids = init_pid_arr(cmd);
		}
	}
	close(pipe_fd[READ]);
	i = 0;
	while (pids[i] != -1)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
	free(pids);
	pids = NULL;
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

	if (!look_for_heredocs(ast)) 	//chercher heredocs dans tout l'arbre (même derrière || et &&) et les lancer
		return (0);					//lancer look_for_heredocs à l'exit des syntax errors (<< lim cat < >)
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
	if (!tree_traversal(ast))
		return (0);
	return (1);
}
