/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 12:19:28 by agranger          #+#    #+#             */
/*   Updated: 2022/09/23 12:28:42 by agranger         ###   ########.fr       */
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

void	go_to_redir_node(t_node **node, int *in, int *out)
{
	t_node	*prev;

	prev = NULL;
	*in = 1;
	*out = 1;
	while (*node && !is_chevron((*node)->type))
	{
		if (prev && prev == (*node)->right)
			*in = 0;
		if ((*node)->type == PIPE && prev == (*node)->left)
			*out = 0;
		prev = *node;
		*node = (*node)->parent;
	}
}
