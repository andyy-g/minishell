/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 21:36:23 by agranger          #+#    #+#             */
/*   Updated: 2022/10/08 18:12:43 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_pipe_cmd(t_node *node)
{
	while (node->parent && node->parent->type != AND
		&& node->parent->type != OR)
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
		return (node);
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

void	move_to_first_cmd(t_node **ast)
{
	while ((*ast)->left)
		*ast = (*ast)->left;
}
