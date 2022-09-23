/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_file_in_out.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 10:37:39 by agranger          #+#    #+#             */
/*   Updated: 2022/09/23 10:41:28 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	set_heredoc(t_node *node, t_node *cmd)
{
	if (cmd && node->heredoc)
		cmd->fd_in = node->heredoc[READ];
	return (1);
}
