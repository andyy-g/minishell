/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 11:10:58 by charoua           #+#    #+#             */
/*   Updated: 2022/08/30 11:18:42 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_syntax_error(t_dblist *list, int bracket)
{
	t_pars	*tmp;

	tmp = list->first;
	if (bracket == 1)
		return (-4);
	else if (bracket != 0)
		return (-5);
	while (tmp)
	{
		if (tmp->token == FILE_IN || tmp->token == FILE_OUT \
			|| tmp->token == FILE_OUT_APP)
		{
			if ((tmp->next && (tmp->next->token != WORD \
				&& tmp->next->token != FD)) || !(tmp->next))
				return (-7);
		}
		tmp = tmp->next;
	}
	return (1);
}

void	ft_error_redir(t_dblist **list)
{
	t_pars	*tmp;

	tmp = (*list)->first;
	while (tmp)
	{
		if (tmp->token == FILE_IN || tmp->token == FILE_OUT \
			|| tmp->token == FILE_OUT_APP)
		{
			if (tmp->next->str && (tmp->next->token != WORD \
				&& tmp->next->token != FD))
				printf("bash: syntax error near unexpected token `%s'\n", \
				tmp->next->str);
			else
				printf("bash: syntax error near unexpected token `newline'\n");
		}
		tmp = tmp->next;
	}
}

void	ft_error(int err, t_dblist **list)
{
	if (err == -1)
		printf("> bash: unexpected EOF while looking for matching `''\n");
	else if (err == -6)
		printf("> bash: unexpected EOF while looking for matching `\"'\n");
	else if (err == -3)
		printf("bash: syntax error near unexpected token `)'\n");
	else if (err == -4)
		printf("bash: syntax error the closing bracket `)' is missing\n");
	else if (err == -5)
		printf("bash: syntax error near unexpected token `('\n");
	else if (err == -7)
		ft_error_redir(list);
}
