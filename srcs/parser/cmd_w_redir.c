/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_w_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 17:07:54 by agranger          #+#    #+#             */
/*   Updated: 2022/09/22 17:10:49 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*is_cmd_redir(t_pars **token, int *status)
{
	t_node	*ret;
	t_pars	*save;

	save = *token;
	ret = cmd_redir(token, status);
	if (!*status)
	{
		free_nodes(ret, NULL, NULL, NULL);
		return (NULL);
	}
	if (ret)
		return (put_next_node_above(ret, status, token, &cmd_redir));
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
		return (free_nodes(NULL, NULL, nodes[LCMD], NULL));
	}
	nodes[OP] = ast_create_node((*token)->token, token);
	if (!nodes[OP])
		return (free_nodes(NULL, NULL, nodes[LCMD], status));
	nodes[RCMD] = file(token, status);
	if (!nodes[RCMD] || !*status)
	{
		*token = save;
		return (free_nodes(nodes[OP], nodes[RCMD], nodes[LCMD], NULL));
	}
	return (ast_add_children(nodes[OP], nodes[LCMD], nodes[RCMD]));
}
