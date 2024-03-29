/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 11:10:58 by charoua           #+#    #+#             */
/*   Updated: 2022/10/06 12:02:59 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_line(char *str, t_pars *pars)
{
	int	i;

	i = 0;
	if (str[i + 1] && str[i + 1] == '|')
	{
		pars->token = OR;
		return (2);
	}
	pars->token = PIPE;
	return (1);
}

int	ft_input(char *str, t_pars *pars)
{
	int	i;

	i = 0;
	if (str[i + 1] && str[i + 1] == '<')
	{
		pars->token = HEREDOC;
		return (2);
	}
	else
	{
		pars->token = FILE_IN;
		return (1);
	}
}

int	ft_output(char *str, t_pars *pars)
{
	int	i;

	i = 0;
	if (str[i + 1] && str[i + 1] == '>')
	{
		pars->token = FILE_OUT_APP;
		return (2);
	}
	else
	{
		pars->token = FILE_OUT;
		return (1);
	}
}

int	ft_and(char *str, t_pars *pars)
{
	int	i;

	i = 0;
	if (str[i + 1] && str[i + 1] == '&')
	{
		pars->token = AND;
		return (2);
	}
	return (0);
}

int	ft_bracket(char c, t_pars *pars, int *bracket)
{
	if (c == '(')
	{
		(*bracket)++;
		pars->token = LPAR;
	}
	else
	{
		(*bracket)--;
		pars->token = RPAR;
	}
	return (1);
}
