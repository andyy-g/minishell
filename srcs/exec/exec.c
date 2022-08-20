/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:19:50 by agranger          #+#    #+#             */
/*   Updated: 2022/08/20 13:35:40 by agranger         ###   ########.fr       */
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

bool	is_builtin(char *cmd)
{
	if (cmd_is(cmd, "echo")
		|| cmd_is(cmd, "pwd")
		|| cmd_is(cmd, "cd")
		|| cmd_is(cmd, "export")
		|| cmd_is(cmd, "unset")
		|| cmd_is(cmd, "env")
		|| cmd_is(cmd, "exit"))
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
	(void)node;
	return (false);
}

bool	is_last_cmd(t_node *node)
{
	(void)node;
	return (false);
}

/************************/

bool	check_logical_node(t_node *node)
{
	//if redir -> up
	//if !parent
	//if pipe -> true
	//if and || or -> fct wait + check exit status :
	//if and 
	//	if g_exit_status == 0 -> true
	//else
	//	-> false
	//if or
	//	if g_exit_status == 0 -> false
	(void)node;
	return (true);
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

int	manage_file_in_out(t_node *node) // NORME
{
	int		fd;
	t_node	*cmd;

	cmd = node;
	node = node->parent;
	while (node && is_chevron(node->type))
	{
		if (node->type == FILE_IN)
		{
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
		}
		if (node->type == FILE_OUT)
		{
			fd = open(node->right->cmd[0], O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, S_IROTH  | S_IRGRP | S_IRUSR | S_IWUSR);
			if (fd == -1)
			{
				perror("open");
				return (0);
			}
			if (cmd->fd_out != 1)
				close(cmd->fd_out);
			cmd->fd_out = fd;
		}
		if (node->type == FILE_OUT_APP)
		{
			fd = open(node->right->cmd[0], O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, S_IROTH | S_IRGRP | S_IRUSR | S_IWUSR);
			if (fd == -1)
			{
				perror("open");
				return (0);
			}
			if (cmd->fd_out != 1)
				close(cmd->fd_out);
			cmd->fd_out = fd;
		}
		node = node->parent;
	}
	return (1);
}

int	init_fd(t_node *node)
{
	int	pipe_fd[2];
	int	ret;

	ret = manage_file_in_out(node);
	if (ret == 0 || ret == 2)
		return (ret);
	if (is_uniq_cmd(node)) // if lastcmd is_uniq == true
		return (1);
	node->is_pipe = true;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	//init fd_in et fd_out if pipe
	//fct utiles : is_first_cmd / is_last_cmd
	//enum READ WRITE
	return (1);
}

int	exec_cmd(t_node *node)
{
	int	ret;
	int	stdin_cpy;
	int stdout_cpy;

	stdin_cpy = -1;
	stdout_cpy = -1;
	if (node->fd_in != STDIN_FILENO)
	{
		stdin_cpy = dup(STDIN_FILENO);
		dup2(node->fd_in, STDIN_FILENO);
	}
	if (node->fd_out != STDOUT_FILENO)
	{
		stdout_cpy = dup(STDOUT_FILENO);
		dup2(node->fd_out, STDOUT_FILENO);
	}
	ret = 1;
	if (cmd_is(node->cmd[0], "echo"))
		ret = exec_builtin(node, &ft_echo);
	else if (cmd_is(node->cmd[0], "pwd"))
		printf("PWD\n");
	//ret = exec_builtin(node, &ft_pwd);
	else if (cmd_is(node->cmd[0], "cd"))
		printf("CD\n");
	//ret = exec_builtin(node, &ft_cd);
	else if (cmd_is(node->cmd[0], "export"))
		printf("EXPORT\n");
	//ret = exec_builtin(node, &ft_export);
	else if (cmd_is(node->cmd[0], "unset"))
		printf("UNSET\n");
	//ret = exec_builtin(node, &ft_unset);
	else if (cmd_is(node->cmd[0], "env"))
		printf("ENV\n");
	//ret = exec_builtin(node, &ft_env);
	else if (cmd_is(node->cmd[0], "exit"))
		printf("EXIT\n");
	//ret = exec_builtin(node, &ft_exit);
	else
		ret = exec_bin(node);
	if (node->fd_in != STDIN_FILENO)
	{
		close(node->fd_in);
		dup2(stdin_cpy, STDIN_FILENO);
		close(stdin_cpy);
	}
	if (node->fd_out != STDOUT_FILENO)
	{
		close(node->fd_out);
		dup2(stdout_cpy, STDOUT_FILENO);
		close(stdout_cpy);
	}
	return (ret);
}

int	fork_process(t_node *ast)
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
		//if (ast->is_pipe)
		//dup
		if (!exec_cmd(ast))
			return (0);
	}
	else
	{
		if (ast->fd_in != 0)
			close(ast->fd_in);
		if (ast->fd_out != 1)
			close(ast->fd_out);
	}
	return (1);
}

int	tree_traversal(t_node *cmd)
{
	int	ret;

	while (cmd)
	{
		ret = init_fd(cmd);
		if (ret == 2)
		{
			cmd = next_cmd(cmd);
			continue ;
		}
		if (!ret || !fork_process(cmd))
			return (0);
		if (!check_logical_node(cmd->parent))
			break ;
		cmd = next_cmd(cmd);
	}
	return (1);
}

void	move_to_first_cmd(t_node **ast)
{
	while ((*ast)->left)
		*ast = (*ast)->left;
}

int	exec(t_node *ast)
{
	int	status;
	int	ret;

	if (!look_for_heredocs(ast)) 	//chercher heredocs dans tout l'arbre (même derrière || et &&) et les lancer
		return (0);					//lancer look_for_heredocs à l'exit des syntax errors (<< lim cat < >)
	move_to_first_cmd(&ast);
	if (is_builtin(ast->cmd[0]) && is_uniq_cmd(ast)) 	//conditions supplémentaires ? 
	{													//(export ? redir out ? ...)
		ret = init_fd(ast);
		if (ret == 2)
			return (1);
		if (!ret || !exec_cmd(ast))
			return (0);
		return (1);
	}
	if (!tree_traversal(ast))
		return (0);
	while (wait(&status) > 0)
		;
	return (1);
}
