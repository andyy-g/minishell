/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 11:10:58 by charoua           #+#    #+#             */
/*   Updated: 2022/06/11 14:41:20 by charoua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_dblist(t_dblist *list)
{
	int		i;
	t_pars	*print;

	i = 1;
	print = list->first;
	while (print->next)
	{
		printf("Mot %d = -%s- et token = %d\n", i, print->str, print->token);
		print = print->next;
		i++;
	}
	printf("\n");
}

int	ft_create_str(char *str, int size, t_pars **pars, t_dblist **list)
{
	int	i;
	int	length;

	i = 0;
	length = size - 2 * ((*pars)->db_quote + (*pars)->sp_quote);
	(*pars)->str = (char *)malloc(sizeof(char) * (length + 1));
	if ((*pars)->str)
	{
		while (i < length)
		{
			((*pars)->str)[i] = str[i + (*pars)->db_quote + (*pars)->sp_quote];
			i++;
		}
		((*pars)->str)[i] = '\0';
		(*list)->last = (*pars);
		(*pars)->next = ft_create_pars(*pars);
		(*pars) = (*pars)->next;
		(*pars)->next = NULL;
	}
	else
		return (0);
	return (1);
}

int	ft_token_size(char *str, char c, t_pars **pars)
{
	int	i;

	i = 0;
	if (c == 34 || c == 39)
		i = ft_quote(str, c, &(*pars));
	else if (c == '|')
		i = ft_line(str, &(*pars));
	else if (c == '<')
		i = ft_input(str, &(*pars));
	else if (c == '>')
		i = ft_output(str, &(*pars));
	else if (c == '&')
		i = ft_and(str, &(*pars));
	if (c != '\n' && c != ' ' && c != '\t' && i == 0)
		i = ft_word(str, &(*pars));
	return (i);
}

int	ft_add_pars(char *str, t_pars **pars, t_dblist **list)
{
	int	i;
	int	j;

	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		j = ft_token_size(str + i, str[i], &(*pars));
		if (j == -1)
			return (0);
		else if (j > 0)
		{
			ft_create_str(str + i, j, &(*pars), &(*list));
			i = i + j;
		}
		else
			i++;
	}
	ft_print_dblist(*list);
	return (1);
}
