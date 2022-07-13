/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 15:40:08 by agranger          #+#    #+#             */
/*   Updated: 2022/07/13 18:17:45 by agranger         ###   ########.fr       */
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
	t_node	*redir;
	t_node	*right;
	t_node	*left;
	t_pars	*save;

	save = *token;
	left = cmd(token, status);
	if (!*status)
		return (NULL);
	if (!(*token) || !(*token)->str
		|| ((*token)->token < 4 || (*token)->token > 7))
	{
		*token = save;
		return (clean_before_backtrack(NULL, NULL, left));
	}
	redir = ast_create_node((*token)->token, token);
	if (!redir)
	{
		*status = 0;
		return (clean_before_backtrack(NULL, NULL, left));
	}
	right = file(token, status);
	if (!right)
	{
		*token = save;
		return (clean_before_backtrack(redir, right, left));
	}
	if (!*status)
		return (clean_before_backtrack(redir, right, left));
	ast_add_children(redir, left, right);
	return (redir);
}

t_node	*cmd_redir2(t_pars **token, int *status)
{
	return (cmd(token, status));
}
