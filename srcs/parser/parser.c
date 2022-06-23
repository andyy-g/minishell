/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 10:13:02 by agranger          #+#    #+#             */
/*   Updated: 2022/06/23 17:57:25 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	link_to_tree(t_node **root, t_node *node, bool is_cmd)
{
	t_node	*tmp;

	if (!*root)
	{
		*root = node;
		return ;
	}
	else
	{
		if (is_cmd)
			(*root)->right = node;
		else
		{
			tmp = *root;
			*root = node;
			(*root)->left = tmp;
		}
	}
}

t_node	*create_ast(t_pars **token, bool expr_bracket, int *status)
{
	t_node	*node;
	t_node	*root;

	root = NULL;
	node = NULL;
	while (*token && (*token)->str)
	{
		if ((*token)->token == PIPE || (*token)->token == OR
				||(*token)->token == AND)
		{
			node = ast_create_node((*token)->token, NULL);
			if (!node)
			{
				ast_delete_nodes(root);
				*status = 0;
				return (NULL);
			}
			*token = (*token)->next;
			link_to_tree(&root, node, false);
		}
		else if (((*token)->token != LPAR && (*token)->token != RPAR)
			|| !expr_bracket)
		{
			node = create_cmd(token, status);
			if (!*status)
			{
				ast_delete_nodes(root);
				*status = 0;
				return (NULL);
			}
			link_to_tree(&root, node, true);
		}
		else if ((*token)->token == LPAR)
			*token = (*token)->next;
		else if((*token)->token == RPAR && expr_bracket)
			return (root);

	}
	return (root);
}

int	parser(t_node **ast, t_pars *token)
{
	int	status;

	status = 1;
	*ast = create_ast(&token, false, &status);
	if (!status)
		return (0);
	return (1);
}
