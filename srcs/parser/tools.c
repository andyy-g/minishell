/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 22:27:02 by agranger          #+#    #+#             */
/*   Updated: 2022/07/16 14:56:01 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redir_token(t_pars *token)
{
	int	type;

	if (!token || !token->str)
		return (false);
	type = token->token;
	if (type == WORD
		|| type == PIPE
		|| type > FILE_OUT_APP)
		return (false);
	return (true);
}

bool	is_chevron(int type)
{
	if (type < FILE_IN || type > FILE_OUT_APP)
		return (false);
	return (true);
}

char	**set_status_error(int *status, char **ret)
{
	*status = 0;
	return (ret);
}

void	*free_nodes(t_node *redir, t_node *right, t_node *left, int *status)
{
	if (left)
		ast_delete_nodes(left);
	if (right)
		ast_delete_nodes(right);
	if (redir)
		ast_delete_nodes(redir);
	if (status)
		*status = 0;
	return (NULL);
}

bool	is_brackets(int type)
{
	if (type == LPAR
		|| type == RPAR)
		return (true);
	return (false);
}
