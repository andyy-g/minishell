/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 22:27:02 by agranger          #+#    #+#             */
/*   Updated: 2022/07/13 23:05:34 by agranger         ###   ########.fr       */
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

bool	is_chevron(t_pars *token)
{
	int	type;

	if (!token || !token->str)
		return (false);
	type = token->token;
	if (type < FILE_IN || type > FILE_OUT_APP)
		return (false);
	return (true);
}
