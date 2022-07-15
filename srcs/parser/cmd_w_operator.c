/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_w_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 15:40:08 by agranger          #+#    #+#             */
/*   Updated: 2022/07/14 16:50:35 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*create_cmd(t_pars **token, int *status)
{
	t_node	*ret;
	t_pars	*save;

	save = *token;
	ret = cmd_pipe(token, status);
	if (ret || !*status)
		return (ret);
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
	if (!(*token) || (*token)->token != PIPE)
	{
		*token = save;
		return (clean_before_backtrack(NULL, NULL, nodes[LCMD]));
	}
	nodes[OP] = ast_create_node((*token)->token, token);
	if (!nodes[OP])
	{
		*status = 0;
		return (clean_before_backtrack(NULL, NULL, nodes[LCMD]));
	}
	nodes[RCMD] = is_cmd_redir(token, status);
	if (!nodes[RCMD] || !*status)
	{
		*token = save;
		return (clean_before_backtrack(nodes[OP], nodes[RCMD], nodes[LCMD]));
	}
	return (ast_add_children(nodes[OP], nodes[LCMD], nodes[RCMD]));
}

t_node	*is_cmd_redir(t_pars **token, int *status)
{
	t_node	*ret;
	t_pars	*save;

	save = *token;
	ret = cmd_redir(token, status);
	if (ret || !*status)
		return (ret);
	*token = save;
	ret = cmd(token, status);
	if (ret || !*status)
		return (ret);
	*token = save;
	return (NULL);
}

t_node	*cmd_redir(t_pars **token, int *status)
{
	t_node	*nodes[3];
	t_pars	*save;

	save = *token;
	nodes[LCMD] = cmd(token, status);
	if (!*status)
		return (NULL);
	if (!(*token) || !(*token)->str || !is_chevron((*token)->token))
	{
		*token = save;
		return (clean_before_backtrack(NULL, NULL, nodes[LCMD]));
	}
	nodes[OP] = ast_create_node((*token)->token, token);
	if (!nodes[OP])
	{
		*status = 0;
		return (clean_before_backtrack(NULL, NULL, nodes[LCMD]));
	}
	nodes[RCMD] = file(token, status);
	if (!nodes[RCMD] || !*status)
	{
		*token = save;
		return (clean_before_backtrack(nodes[OP], nodes[RCMD], nodes[LCMD]));
	}
	return (ast_add_children(nodes[OP], nodes[LCMD], nodes[RCMD]));
}
