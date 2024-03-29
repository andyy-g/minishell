/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 11:10:58 by charoua           #+#    #+#             */
/*   Updated: 2022/10/07 20:28:39 by agranger         ###   ########.fr       */
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
	check = 0;
	while (str[j] != '\0')
	{
		check = 0;
		if (str[j] == 34 || str[j] == 39)
			check = ft_close_quote(str, str[j], j);
		if (check == -1)
			break ;
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

int	ft_word(char *str, t_dblist **list, t_sa *sig, int i)
{
	char	c;
	int		ret;
	t_pars	*pars;

	pars = (*list)->curr;
	while (str[i] != '\0')
	{
		if (str[i] == 34 || str[i] == 39)
		{
			c = str[i++];
			while (str[i] != c)
				i++;
		}
		if (!ft_check_end(str, i))
			break ;
		i++;
	}
	pars->token = WORD;
	(*list)->curr = pars->prev;
	ret = check_is_heredoc(ft_substr(str, 0, i), sig, list);
	(*list)->curr = pars;
	if (ret == -1 || ret == -2)
		return (ret);
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
