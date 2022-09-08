/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 11:10:58 by charoua           #+#    #+#             */
/*   Updated: 2022/09/01 17:26:10 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_end(char *str, int i)
{
	if (str[i] == '|' || str[i] == '<' || str[i] == '>' \
	|| (str[i] == '&' && str[i + 1] && str[i + 1] == '&') \
	|| str[i] == ' ' || str[i] == '\n' || str[i] == '\t' \
	|| str[i] == '(' || str[i] == ')')
		return (0);
	return (1);
}

int	ft_close_quote(char *str, char c, int i)
{
	int	j;

	j = 1;
	while (str[i + j] != '\0')
	{
		if (str[i + j] == c)
			return (j);
		j++;
	}
	return (-1);
}

int	ft_quote(char *str)
{
	int	j;
	int	check;

	j = 0;
	while (str[j] != '\0')
	{
		check = 0;
		if (str[j] == 34 || str[j] == 39)
			check = ft_close_quote(str, str[j], j);
		if (check == -1)
			break ;
		else
			j = j + check + 1;
	}
	if (check >= 0)
		return (0);
	else
	{
		if (str[j] == 39)
			display_error(ERR_MATCHING_TOK, "'");
		else
			display_error(ERR_MATCHING_TOK, "\"");
		return (-2);
	}
}

int	ft_word(char *str, t_pars **pars)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_check_end(str, i))
			break ;
		i++;
	}
	(*pars)->token = WORD;
	if (!check_is_heredoc((*pars)->prev, ft_substr(str, 0, i)))
		return (-1);
	return (i);
}

void	ft_check_word(t_dblist **list)
{
	t_pars	*pars;
	int		token;

	pars = (*list)->first;
	while (pars)
	{
		if (pars->prev && pars->token == WORD)
		{
			token = (pars->prev)->token;
			if (token == FILE_IN || token == FILE_OUT || token == FILE_OUT_APP)
				pars->token = FD;
			else if (token == HEREDOC)
				pars->token = LIMITOR;
		}
		pars = pars->next;
	}
}
