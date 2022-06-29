/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 10:13:02 by agranger          #+#    #+#             */
/*   Updated: 2022/06/29 19:05:39 by agranger         ###   ########.fr       */
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
/*
void	swap_redirs(t_token *start_redir)

void	put_redirs_in_order(t_pars *token)
{
	t_token	*end;

	end = token;
	while (end->next->str && (end->next->token == WORD || is_redir_token(end->next)))
		end = end->next;
	
	swap_redirs(redir, fd, &end);	
}
*/
t_node	*create_ast(t_pars **token, bool expr_bracket, int *status)
{
	t_node	*node;
	t_node	*root;

	root = NULL;
	node = NULL;
	while (*token && (*token)->str)
	{
		if ((*token)->str && (*token)->token == NONE)
		{
			while ((*token)->str && (*token)->token == NONE)
				*token = (*token)->next;
			continue ;
		}
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
			if (!node)
			{
				printf("bash: syntax error\n");
				return (NULL);
			}
			if (node->left)
				link_to_tree(&root, node, true);
			else
				link_to_tree(&root, node, false);
		}
		else if ((*token)->token == LPAR)
			*token = (*token)->next;
		else if((*token)->token == RPAR && expr_bracket)
			return (root);

	}
	return (root);
}

int	parser(t_node **ast, t_pars *token, int *error)
{
	int	status;

	status = 1;
	//put_redirs_in_order(token);
	*ast = create_ast(&token, false, &status);
	if (!status)
		return (0);
	if (!*ast)
		*error = 1;
	return (1);
}
