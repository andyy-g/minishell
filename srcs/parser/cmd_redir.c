/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 15:40:08 by agranger          #+#    #+#             */
/*   Updated: 2022/07/13 23:02:43 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*create_cmd(t_pars **token, int *status)
{
	t_node	*ret;
	t_pars	*save;

	save = *token;
	ret = cmd_redir1(token, status);
	if (ret || !*status)
		return (ret);
	*token = save;
	ret = cmd_redir2(token, status);
	if (ret || !*status)
		return (ret);
	*token = save;
	return (NULL);
}

void	*clean_before_backtrack(t_node *redir, t_node *right, t_node *left)
{
	if (left)
		ast_delete_nodes(left);
	if (right)
		ast_delete_nodes(right);
	if (redir)
		ast_delete_nodes(redir);
	return (NULL);
}

t_node	*cmd_redir1(t_pars **token, int *status)
{
	t_node	*nodes[3];
	t_pars	*save;

	save = *token;
	nodes[CMD] = cmd(token, status);
	if (!*status)
		return (NULL);
	if (!(*token) || !(*token)->str || !is_chevron(*token))
	{
		*token = save;
		return (clean_before_backtrack(NULL, NULL, nodes[CMD]));
	}
	nodes[REDIR] = ast_create_node((*token)->token, token);
	if (!nodes[REDIR])
	{
		*status = 0;
		return (clean_before_backtrack(NULL, NULL, nodes[CMD]));
	}
	nodes[OUT] = file(token, status);
	if (!nodes[OUT] || !*status)
	{
		*token = save;
		return (clean_before_backtrack(nodes[REDIR], nodes[OUT], nodes[CMD]));
	}
	return (ast_add_children(nodes[REDIR], nodes[CMD], nodes[OUT]));
}

t_node	*cmd_redir2(t_pars **token, int *status)
{
	return (cmd(token, status));
}
