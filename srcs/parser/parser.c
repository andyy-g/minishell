/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 10:13:02 by agranger          #+#    #+#             */
/*   Updated: 2022/06/30 17:36:40 by agranger         ###   ########.fr       */
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

void	swap_redirs(t_pars *redir, t_pars *fd, t_pars **last)
{
	t_pars	*prev;
	t_pars	*next;
	t_pars	*end;

	prev = redir->prev;
	next = fd->next;
	prev->next = next;
	next->prev = prev;
	end = (*last)->next;
	(*last)->next = redir;
	redir->prev = *last;
	fd->next = end;
	if (end->str)
		end->prev = fd;
	*last = fd;
}

void	put_redirs_in_order(t_pars *token)
{
	t_pars	*last;
	t_pars	*tmp;

	last = token;
	while (last->next->str && (last->next->token == WORD || is_redir_token(last->next)))
		last = last->next;
	while (token->str && (token->token == WORD || is_redir_token(token)))
	{
		if (token->token > 3 && token->token < 8)
		{
			tmp = token->next->next;
			if (token->next != last)
				swap_redirs(token, token->next, &last);
			token = tmp;
		}
		else
			token = token->next;
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
			if (node->type >= 4 && node->type <= 7 && !node->left)
				link_to_tree(&root, node, false);
			else
				link_to_tree(&root, node, true);
		}
		else if ((*token)->token == LPAR)
			*token = (*token)->next;
		else if((*token)->token == RPAR && expr_bracket)
			return (root);

	}
	return (root);
}

void	print_tokens(t_pars *print)
{
	int i;

	i = 1;
	while (print->next)
	{
		printf("Mot %d = -%s- et token = %d\n", i, print->str, print->token);
		print = print->next;
		i++;
	}
	printf("\n");
}

int	parser(t_node **ast, t_pars *token, int *error)
{
	int	status;

	status = 1;
	//put_redirs_in_order(token);
	//print_tokens(token);
	*ast = create_ast(&token, false, &status);
	if (!status)
		return (0);
	if (!*ast)
		*error = 1;
	return (1);
}
