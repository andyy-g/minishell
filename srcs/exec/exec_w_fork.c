/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_w_fork.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 10:46:21 by agranger          #+#    #+#             */
/*   Updated: 2022/09/23 12:27:26 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	pids = ft_calloc(nb_cmd + 1, sizeof(*pids));
	pids[nb_cmd] = -1;
	*index_cmd = 0;
	return (pids);
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
	if (node->heredoc)
	{
		close(node->heredoc[0]);
		ft_free(node->heredoc);
	}
	while (node->parent)
		node = node->parent;
	exit_minishell(node);
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
		check_full_path(node, &path, &is_dir, &cmd_not_found);
	if (check_error(node, is_dir, cmd_not_found))
	{
		execve(path, node->cmd, envp);
		which_error(node);
		ft_free(path);
	}
	ft_free(envp);
	close_fds_exec_fail(node);
	exit_minishell(node);
	return (1);
}

int	dup2_if_needed(t_node *node)
{
	if (node->fd_in != STDIN_FILENO)
	{
		if (dup2(node->fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (0);
		}
	}
	if (node->fd_out != STDOUT_FILENO)
	{
		if (dup2(node->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			return (0);
		}
	}
	return (1);
}

int	exec_cmd_fork(t_node *node, pid_t pid)
{
	int	ret;

	if (!dup2_if_needed(node))
		return (0);
	ret = 1;
	if (cmd_is(node->cmd[0], "echo"))
		ret = exec_builtin(node, &ft_echo);
	else if (cmd_is(node->cmd[0], "pwd"))
		printf("PWD\n");
	else if (cmd_is(node->cmd[0], "export"))
		printf("EXPORT\n");
	else if (cmd_is(node->cmd[0], "env"))
		printf("ENV\n");
	else if (cmd_is(node->cmd[0], "cd"))
		printf("CD\n");
	else if (cmd_is(node->cmd[0], "unset"))
		printf("UNSET\n");
	else if (cmd_is(node->cmd[0], "exit"))
		ret = exec_builtin(node, &ft_exit);
	else
		ret = exec_bin(node);
	if (pid == 0)
		exit_child_builtin(node);
	return (ret);
}
