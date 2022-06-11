/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 11:10:58 by charoua           #+#    #+#             */
/*   Updated: 2022/06/11 13:01:33 by charoua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_quote(char *str, char c, t_pars **pars)
{
	int	j;

	j = 1;
	while (str[j] != '\0' && str[j] != c)
		j++;
	if (str[j] == c)
	{
		if (c == 39)
			(*pars)->sp_quote = 1;
		else
			(*pars)->db_quote = 1;
		(*pars)->token = WORD;
		return (j + 1);
	}
	else
		return (-1);
}

int	ft_word(char *str, t_pars **pars)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '|' || str[i] == '<' || str[i] == '>' \
			|| (str[i] == '&' && str[i + 1] && str[i + 1] == '&') \
			|| str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
			break ;
		i++;
	}
	(*pars)->token = WORD;
	return (i);
}
