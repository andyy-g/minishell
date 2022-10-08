/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_simple.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 15:39:00 by agranger          #+#    #+#             */
/*   Updated: 2022/10/08 18:11:51 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*cmd(t_pars **token, int *status)
{
	t_node	*ret;
	t_pars	*save;

	save = *token;
	ret = cmd_brackets(token, status);
	if (ret || !*status)
		return (ret);
	*token = save;
	ret = cmd_simple(token, status);
	if (ret || !*status)
		return (ret);
	*token = save;
	return (NULL);
}

t_node	*cmd_brackets(t_pars **token, int *status)
{
	t_node	*ret;
	t_pars	*save;

	save = *token;
	if (!(*token) || (*token)->token != LPAR)
	{
		*token = save;
		return (NULL);
	}
	ret = create_ast(token, true, status);
	if (!*status)
	{
		ast_delete_nodes(ret);
		return (NULL);
	}
	if (!(*token) || (*token)->token != RPAR)
	{
		ast_delete_nodes(ret);
		*token = save;
		return (NULL);
	}
	*token = (*token)->next;
	return (ret);
}

t_node	*cmd_simple(t_pars **token, int *status)
{
	t_node	*cmd;

	(void)status;
	if (*token && (*token)->str
		&& (*token)->token != WORD && (*token)->token != NONE)
		return (NULL);
	cmd = ast_create_node((*token)->token, token);
	if (!cmd)
		*status = 0;
	return (cmd);
}

t_node	*file(t_pars **token, int *status)
{
	t_node	*file;

	(void)status;
	if (!*token || !(*token)->str
		|| ((*token)->token != FD && (*token)->token != LIMITOR))
		return (NULL);
	file = ast_create_node((*token)->token, token);
	if (!file)
		*status = 0;
	return (file);
}
