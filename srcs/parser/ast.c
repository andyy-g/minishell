/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 17:50:23 by agranger          #+#    #+#             */
/*   Updated: 2022/06/10 18:27:57 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 	typedef struct	s_node
	{
		char			*name;
		t_toktype		type;
		bool			exp;
		struct s_node	*parent;
		struct s_node	*left;
		struct s_node	*right;
	}	t_node;
*/

/* EXPANSION
	
   sans quotes
   double quotes
   heredoc limitor
 
*/
bool	must_be_expand(t_token **token)
{
	
}

t_node	*ast_create_node(t_token **token)
{
	t_node	*ret;

	ret = malloc(*ret);
	if (!ret)
		return (NULL);
	ret->name = ft_strdup((*token)->name);
	if (!ret->name)
		return (NULL);
	ret->type = (*token)->type;
	ret->exp = must_be_expand(token);
	ret->parent = NULL;
	ret->left = NULL;
	ret->right = NULL;
	return (ret);
}

void	ast_add_children(t_node *parent, t_node *left_child, t_node *rigth_child);
{
	parent->left = left_child;
	parent->right = right_child;
	left_child->parent = parent;
	right_child->parent = parent;
}

void	ast_delete_nodes(t_node *node);
{
	if (!node)
		return ;
	if (node->name)
	{
		free(node->name);
		node->name = NULL;
	}
	ast_delete_nodes(node->left);
	ast_delete_nodes(node->right);
	free(node);
}

void	ast_add_arg_cmd(t_node **first, t_node *new);
{
	t_node	*last;

	last = *first;
	while (last->right)
		last = last->right;
	last->right = new;
}
