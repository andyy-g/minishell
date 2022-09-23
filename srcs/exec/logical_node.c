/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 11:38:50 by agranger          #+#    #+#             */
/*   Updated: 2022/09/23 14:26:40 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = WTERMSIG(status) + 128;
	else if (WIFSTOPPED(status))
		status = WSTOPSIG(status) + 128;
	return (status);
}

void	next_logical_node(t_node **node)
{
	t_node	*prev;

	prev = (*node)->right;
	if (((*node)->type == AND || (*node)->type == OR))
	{
		while (*node && ((*node)->type == AND || (*node)->type == OR)
			&& prev == (*node)->right)
		{
			prev = *node;
			*node = (*node)->parent;
		}
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
	while ((*node)->parent && ((*node)->parent->right == *node
			|| is_chevron((*node)->parent->type)))
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
	int	status;

	next_logical_node(node);
	if (!*node || (*node)->type == PIPE)
		return (false);
	status = get_status_last_process(pids);
	return (check_status(node, status));
}