/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 13:57:34 by agranger          #+#    #+#             */
/*   Updated: 2022/09/27 17:25:44 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_new_tokens(t_pars **exp, t_pars *next, char **split)
{
	int		i;
	t_pars	*tmp;

	i = 0;
	while (split[i])
	{
		tmp = ft_create_pars(*exp);
		if (!tmp)
			return (0);
		tmp->str = split[i];
		tmp->token = WORD;
		tmp->prev = *exp;
		tmp->next = next;
		if (*exp)
			(*exp)->next = tmp;
		if (next)
			next->prev = tmp;
		*exp = tmp;
		i++;
	}
	return (1);
}

int	split_token(t_pars **exp)
{
	char	**split;
	t_pars	*prev;
	t_pars	*next;

	prev = (*exp)->prev;
	next = (*exp)->next;
	split = ft_split((*exp)->str, ' ');
	if (!split)
		return (0);
	remove_pars(exp);
	*exp = prev;
	if (!create_new_tokens(exp, next, split))
		return (0);
	free(split);
	return (1);
}

bool	must_be_split(t_pars *exp, bool dquote)
{
	if (dquote)
		return (false);
	if (!is_charset_str(exp->str, " \t\n"))
		return (false);
	return (true);
}

int	word_splitting(t_pars **exp, bool dquote)
{
	if (must_be_split(*exp, dquote))
	{
		if (!split_token(exp))
			return (0);
	}
	return (1);
}
