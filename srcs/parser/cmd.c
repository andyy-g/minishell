/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 17:21:37 by agranger          #+#    #+#             */
/*   Updated: 2022/07/05 14:58:52 by agranger         ###   ########.fr       */
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
	ret = cmd_redir3(token, status);
	if (ret || !*status)
		return (ret);
	*token = save;
	return (NULL);
}

void	add_word_to_cmd(t_pars *token, t_node *node, int *status)
{
	int	i;

	i = 0;
	if (node->type != WORD)
		return ;
	while (node->cmd[i])
		i++;
	while (token->str && is_redir_token(token))
		token = token->next;
	if (token->str && token->token == WORD)
	{
		while (token->str && token->token == WORD)
		{
			node->cmd[i] = ft_strdup(token->str);
			if (!node->cmd[i])
			{
				*status = 0;
				return ;
			}
			token->token = NONE;
			i++;
			token = token->next;
		}
		node->cmd[i] = NULL;
	}
}

t_node	*cmd_redir1(t_pars **token, int *status)
{
	t_node	*redir;
	t_node	*right;
	t_node	*left;
	t_pars	*save;

	save = *token;
	left = cmd(token, status);
	if (!left || !*status)
	{
		*token = save;
		return (NULL);
	}
	if (!(*token) || !(*token)->str
			|| ((*token)->token < 4 || (*token)->token > 7))
	{
		ast_delete_nodes(left);
		*token = save;
		return (NULL);
	}
	redir = ast_create_node((*token)->token, NULL);
	if (!redir)
	{
		ast_delete_nodes(left);
		*token = save;
		*status = 0;
		return (NULL);
	}
	*token = (*token)->next;
	right = file(token, status);
	if (!right || !*status)
	{
		ast_delete_nodes(left);
		ast_delete_nodes(redir);
		*token = save;
		return (NULL);
	}
	add_word_to_cmd(*token, left, status);
	if (!*status)
	{
		ast_delete_nodes(left);
		ast_delete_nodes(right);
		ast_delete_nodes(redir);
		*token = save;
		return (NULL);
	}
	ast_add_children(redir, left, right);
	return (redir);
}

t_node	*cmd_redir2(t_pars **token, int *status)
{
	t_node	*redir;
	t_node	*right;
	t_pars	*save;

	save = *token;
	if (!(*token) || !(*token)->str
			|| ((*token)->token < 4 || (*token)->token > 7))
	{
		*token = save;
		return (NULL);
	}
	redir = ast_create_node((*token)->token, NULL);
	if (!redir)
	{
		*token = save;
		*status = 0;
		return (NULL);
	}

	*token = (*token)->next;
	right = file(token, status);
	if (!right || !*status)
	{
		ast_delete_nodes(redir);
		*token = save;
		return (NULL);
	}
	ast_add_children(redir, NULL, right);
	return (redir);

}

t_node	*cmd_redir3(t_pars **token, int *status)
{
	return (cmd(token, status));
}

t_node	*cmd(t_pars **token, int *status)
{
	t_node	*ret;
	t_pars	*save;

	save = *token;
	ret = cmd1(token, status);
	if (ret || !*status)
		return (ret);
	*token = save;
	ret = cmd2(token, status);
	if (ret || !*status)
		return (ret);
	*token = save;
	return (NULL);
}

t_node	*cmd1(t_pars **token, int *status)
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

t_node	*cmd2(t_pars **token, int *status)
{
	t_node	*cmd;

	(void)status;
	if (!*token || !(*token)->str || (*token)->token != WORD)
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
