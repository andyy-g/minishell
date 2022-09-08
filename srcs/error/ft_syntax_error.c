/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 11:10:58 by charoua           #+#    #+#             */
/*   Updated: 2022/09/08 22:11:33 by agranger         ###   ########.fr       */
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
	{
		display_error(ERR_UNEXPECTED_TOK, curr->str);
		return (0);
	}
	if (curr->token == LPAR && prev && (prev->token == WORD || prev->token == RPAR))
	{
		display_error(ERR_UNEXPECTED_TOK, curr->str);
		return (0);
	}
	if (prev && prev->token == RPAR && curr->token == WORD)
	{
		display_error(ERR_UNEXPECTED_TOK, curr->str);
		return (0);
	}
	if (bracket < 0)
	{
		display_error(ERR_UNEXPECTED_TOK, curr->str);
		return (0);
	}	
	if (!*str && bracket == 1)
	{
		display_error(ERR_MATCHING_TOK, ")");
		return (0);
	}	

	return (1);
}

int	check_syntax(t_pars *curr, char *str, int i, int bracket)
{
	t_pars	*prev;

	prev = curr->prev;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	if (!check_redir_syntax(curr, prev, &str[i]))
		return (0);
	if (!check_op_syntax(curr, prev, &str[i]))
		return (0);
	if (!check_bracket_syntax(curr, prev, bracket, &str[i]))
		return (0);
	return (1);	
}

void	display_error(t_err err, char *arg)
{
	ft_putstr_fd("minishell: ", 2);
	if (err == ERR_UNEXPECTED_TOK)
	{
		ft_putstr_fd("syntax error near unexpected token `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("'", 2);
	}
	if (err == ERR_MATCHING_TOK)
	{
		ft_putstr_fd("unexpected EOF while looking for matching `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("'", 2);
	}
	if (err == ERR_PERM_DENIED)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": Permission denied", 2);
	}
	if (err == ERR_CMD_NOT_FOUND)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": command not found", 2);
	}
	if (err == ERR_IS_DIR)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": Is a directory", 2);
	}
	if (err == ERR_NO_FILE)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": No such file or directory", 2);
	}
	ft_putstr_fd("\n", 2);
}
