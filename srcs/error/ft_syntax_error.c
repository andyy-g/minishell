/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 11:10:58 by charoua           #+#    #+#             */
/*   Updated: 2022/10/05 17:16:01 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_op(int toktype)
{
	if (toktype == PIPE
		|| toktype == AND
		|| toktype == OR)
		return (true);
	return (false);
}

int	check_redir_syntax(t_pars *curr, t_pars *prev, char *str)
{
	if (prev && is_chevron(prev->token))
	{
		if (curr->token != WORD)
		{
			display_error(ERR_UNEXPECTED_TOK, curr->str);
			return (0);
		}
	}
	if (is_chevron(curr->token) && !*str)
	{
		display_error(ERR_UNEXPECTED_TOK, "newline");
		return (0);
	}
	return (1);
}

int	check_op_syntax(t_pars *curr, t_pars *prev, char *str)
{
	if (!prev && is_op(curr->token))
	{
		display_error(ERR_UNEXPECTED_TOK, curr->str);
		return (0);
	}
	if (prev && is_op(prev->token))
	{
		if (curr->token != WORD && !is_chevron(curr->token)
			&& curr->token != LPAR)
		{
			display_error(ERR_UNEXPECTED_TOK, curr->str);
			return (0);
		}
	}
	if (is_op(curr->token) && !*str)
	{
		display_error(ERR_UNEXPECTED_TOK, "newline");
		return (0);
	}
	return (1);
}

int	check_bracket_syntax(t_pars *curr, t_pars *prev, int bracket, char *str)
{
	if (curr->token == RPAR && prev && prev->token == LPAR)
		display_error(ERR_UNEXPECTED_TOK, curr->str);
	else if (curr->token == LPAR && prev \
	&& (prev->token == WORD || prev->token == RPAR))
		display_error(ERR_UNEXPECTED_TOK, curr->str);
	else if (prev && prev->token == RPAR && curr->token == WORD)
		display_error(ERR_UNEXPECTED_TOK, curr->str);
	else if (bracket < 0)
		display_error(ERR_UNEXPECTED_TOK, curr->str);
	else if (!*str && bracket == 1)
		display_error(ERR_MATCHING_TOK, ")");
	else
		return (1);
	return (0);
}

int	check_syntax(t_pars *curr, char *str, int i, int bracket)
{
	t_pars	*prev;
	int		ret;

	prev = curr->prev;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	ret = check_redir_syntax(curr, prev, &str[i]);
	if (ret)
		ret = check_op_syntax(curr, prev, &str[i]);
	if (ret)
		ret =check_bracket_syntax(curr, prev, bracket, &str[i]);
	if (!ret)
		g_exit_status = 2;
	return (ret);
}
