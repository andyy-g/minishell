/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 11:10:58 by charoua           #+#    #+#             */
/*   Updated: 2022/09/22 14:44:32 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_dblist(t_dblist *list)
{
	int		i;
	t_pars	*print;

	i = 1;
	print = list->first;
	printf("BEGINNING\n");
	while (print->str)
	{
		printf("Mot %d = -%s- et token = %d\n", i, print->str, print->token);
		print = print->next;
		i++;
	}
	printf("\nEND\n");
	print = list->last;
	i = 1;
	while (print)
	{
		printf("Mot %d = -%s- et token = %d\n", i, print->str, print->token);
		print = print->prev;
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
		if (!(*pars)->next)
			return (0);
		(*pars) = (*pars)->next;
		(*pars)->next = NULL;
	}
	else
		return (0);
	return (1);
}

int	ft_token_size(char *str, char c, t_pars **pars, int *bracket)
{
	int	i;

	i = ft_quote(str);
	if (c == '|')
		i = ft_line(str, &(*pars));
	else if (c == '<')
		i = ft_input(str, &(*pars));
	else if (c == '>')
		i = ft_output(str, &(*pars));
	else if (c == '&')
		i = ft_and(str, &(*pars));
	else if (c == '(' || c == ')')
		i = ft_bracket(c, &(*pars), bracket);
	if (c != '\n' && c != ' ' && c != '\t' && i == 0)
		i = ft_word(str, &(*pars));
	return (i);
}

int	ft_add_lex(char *str, t_pars **pars, t_dblist **list)
{
	int	i;
	int	j;
	int	bracket;

	i = 0;
	bracket = 0;
	while (str[i] != '\0')
	{
		j = ft_token_size(str + i, str[i], pars, &bracket);
		if (j == -1 || j == -2)
			return (j + 1);
		else if (j > 0)
		{
			if (!ft_create_str(str + i, j, pars, &(*list)))
				return (0);
			if (!check_syntax((*pars)->prev, str, i + j, bracket))
				return (-1);
			i = i + j;
		}
		else
			i++;
	}
	ft_check_word(&(*list));
	return (1);
}

int	ft_lexer(char *str, t_dblist **list, int *err)
{
	int		error;
	t_pars	*pars;

	*list = create_list();
	if (!*list)
		return (0);
	pars = ft_create_pars(NULL);
	if (!pars)
		return (0);
	(*list)->first = pars;
	(*list)->last = pars;
	error = ft_add_lex(str, &pars, list);
	if (!error)
		return (0);
	if (error == -1)
	{
		*err = 1;
		free_heredoc(pars);
	}
	return (1);
}
