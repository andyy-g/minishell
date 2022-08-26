/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 10:13:02 by agranger          #+#    #+#             */
/*   Updated: 2022/08/26 16:40:21 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	link_to_tree(t_node **root, t_node *node)
{
	t_node	*tmp;

	if (!*root)
	{
		*root = node;
		return ;
	}
	else
	{
		if ((is_chevron(node->type) || node->type == PIPE
				|| node->type == AND || node->type == OR) && !node->left)
		{
			tmp = *root;
			*root = node;
			(*root)->left = tmp;
			tmp->parent = *root;
		}
		else
		{
			(*root)->right = node;
			node->parent = *root;
		}
	}
}

t_node	*create_ast(t_pars **token, bool expr_bracket, int *status)
{
	t_node	*node;
	t_node	*root;

	root = NULL;
	node = NULL;
	while ((*token)->str && !((*token)->token == RPAR && expr_bracket))
	{
		if (!is_brackets((*token)->token) || !expr_bracket)
		{
			node = create_cmd(token, status);
			if (!*status)
				return (free_nodes(root, NULL, NULL, NULL));
			link_to_tree(&root, node);
		}
		else if ((*token)->token == LPAR)
			*token = (*token)->next;
	}
	return (root);
}

int	parser(t_node **ast, t_pars *token, int *error)
{
	int	status;

	status = 1;
	token = put_redirs_in_order(token);
	*ast = create_ast(&token, false, &status);
	if (!status)
		return (0);
	if (!*ast)
		*error = 1;
	return (1);
}
