/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fd_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 10:28:38 by agranger          #+#    #+#             */
/*   Updated: 2022/09/23 12:28:25 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_file_out(t_node *node, t_node *cmd)
{
	if (node->type == FILE_OUT)
		if (!create_file_out(node, cmd))
			return (0);
	if (node->type == FILE_OUT_APP)
		if (!create_file_out_app(node, cmd))
			return (0);
	return (1);
}

int	set_file_in(t_node *node, t_node *cmd)
{
	int	ret;

	if (node->type == FILE_IN)
	{
		ret = file_in_exist(node, cmd);
		if (!ret || ret == 2)
			return (ret);
	}
	if (node->type == HEREDOC)
		if (!set_heredoc(node, cmd))
			return (0);
	return (1);
}

int	check_file_in_out(t_node *node)
{
	t_node	*cmd;
	int		ret;
	int		in;
	int		out;

	cmd = node;
	go_to_redir_node(&node, &in, &out);
	while (node && is_chevron(node->type))
	{
		if (in && (node->type == FILE_IN || node->type == HEREDOC))
		{
			ret = set_file_in(node, cmd);
			if (!ret || ret == 2)
				return (ret);
		}
		if (out && (node->type == FILE_OUT || node->type == FILE_OUT_APP))
		{
			if (!set_file_out(node, cmd))
				return (0);
		}
		node = node->parent;
	}
	return (1);
}

int	init_pipe(t_node *node, int prev_fd, int *pipe_fd, int ret)
{
	node->is_pipe = true;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	if (ret == 2)
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

int	init_fd(t_node *node, int *pipe_fd)
{
	int	ret;
	int	prev_fd;

	prev_fd = -1;
	if (pipe_fd)
		prev_fd = pipe_fd[READ];
	ret = check_file_in_out(node);
	if (!ret)
		return (0);
	if (!is_pipe_cmd(node) || !pipe_fd)
	{
		if (ret == 2)
			return (2);
		return (1);
	}
	return (init_pipe(node, prev_fd, pipe_fd, ret));
}
