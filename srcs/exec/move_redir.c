/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 12:19:28 by agranger          #+#    #+#             */
/*   Updated: 2022/10/11 16:19:42 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	free_pids_exit(pid_t *pids)
{
	free(pids);
	return (0);
}
