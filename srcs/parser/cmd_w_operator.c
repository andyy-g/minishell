/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_w_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 15:40:08 by agranger          #+#    #+#             */
/*   Updated: 2022/10/08 23:06:42 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*put_next_node_above(t_node *ret, int *status, t_pars **token,
		t_node *(*cmd_next_node)(t_pars **token, int *status))
{
	t_node	*tmp;

	if (!(*token)->str)
		return (ret);
	tmp = ret;
	while ((*token)->str)
	{
		tmp = cmd_next_node(token, status);
		if (!*status)
		{
			free_nodes(ret, NULL, NULL, NULL);
			return (NULL);
		}
		if (!tmp)
			break ;
		tmp->left = ret;
		ret->parent = tmp;
		ret = tmp;
	}
	return (ret);
}

t_node	*create_cmd(t_pars **token, int *status)
{
	t_node	*ret;
	t_pars	*save;

	save = *token;
	ret = cmd_logical_operator(token, status);
	if (!*status)
	{
		free_nodes(ret, NULL, NULL, NULL);
		return (NULL);
	}
	if (ret)
		return (put_next_node_above(ret, status, token, &cmd_logical_operator));
	*token = save;
	ret = is_cmd_pipe(token, status);
	if (ret || !*status)
		return (ret);
	*token = save;
	return (NULL);
}

t_node	*cmd_logical_operator(t_pars **token, int *status)
{
	t_node	*nodes[3];
	t_pars	*save;

	save = *token;
	nodes[LCMD] = is_cmd_pipe(token, status);
	if (!*status)
		return (NULL);
	if (!(*token)->str || ((*token)->token != AND && (*token)->token != OR))
	{
		*token = save;
		return (free_nodes(NULL, NULL, nodes[LCMD], NULL));
	}
	nodes[OP] = ast_create_node((*token)->token, token);
	if (!nodes[OP])
	{
		*status = 0;
		return (free_nodes(NULL, NULL, nodes[LCMD], NULL));
	}
	nodes[RCMD] = is_cmd_pipe(token, status);
	if (!nodes[RCMD] || !*status)
	{
		*token = save;
		return (free_nodes(nodes[OP], nodes[RCMD], nodes[LCMD], NULL));
	}
	return (ast_add_children(nodes[OP], nodes[LCMD], nodes[RCMD]));
}

t_node	*is_cmd_pipe(t_pars **token, int *status)
{
	t_node	*ret;
	t_pars	*save;

	save = *token;
	ret = cmd_pipe(token, status);
	if (!*status)
	{
		free_nodes(ret, NULL, NULL, NULL);
		return (NULL);
	}
	if (ret)
		return (put_next_node_above(ret, status, token, &cmd_pipe));
	*token = save;
	ret = is_cmd_redir(token, status);
	if (ret || !*status)
		return (ret);
	*token = save;
	return (NULL);
}

t_node	*cmd_pipe(t_pars **token, int *status)
{
	t_node	*nodes[3];
	t_pars	*save;

	save = *token;
	nodes[LCMD] = is_cmd_redir(token, status);
	if (!*status)
		return (NULL);
	if (!(*token)->str || (*token)->token != PIPE)
	{
		*token = save;
		return (free_nodes(NULL, NULL, nodes[LCMD], NULL));
	}
	nodes[OP] = ast_create_node((*token)->token, token);
	if (!nodes[OP])
	{
		*status = 0;
		return (free_nodes(NULL, NULL, nodes[LCMD], NULL));
	}
	nodes[RCMD] = is_cmd_redir(token, status);
	if (!nodes[RCMD] || !*status)
	{
		*token = save;
		return (free_nodes(nodes[OP], nodes[RCMD], nodes[LCMD], NULL));
	}
	return (ast_add_children(nodes[OP], nodes[LCMD], nodes[RCMD]));
}
