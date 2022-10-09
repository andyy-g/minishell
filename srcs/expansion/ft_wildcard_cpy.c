/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard_cpy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 14:26:43 by charoua           #+#    #+#             */
/*   Updated: 2022/10/09 07:24:08 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_copy_str(char *dir)
{
	int		i;
	int		size;
	char	*new;

	i = 0;
	size = ft_strlen(dir) + 1;
	new = (char *)malloc(sizeof(char) * size);
	if (new)
	{
		while (i < size - 1)
		{
			new[i] = dir[i];
			i++;
		}
		new[i] = '\0';
	}
	else
		return (NULL);
	return (new);
}

int	ft_copy_pars(t_pars **prev, char *dir)
{
	t_pars	*new;
	t_pars	*next;

	new = (t_pars *)malloc(sizeof(t_pars));
	next = (*prev)->next;
	if (new)
	{
		new->str = ft_copy_str(dir);
		new->token = (*prev)->token;
		new->db_quote = (*prev)->db_quote;
		new->sp_quote = (*prev)->sp_quote;
		new->heredoc = NULL;
		new->prev = (*prev);
		new->next = next;
		(*prev)->next = new;
		next->prev = new;
	}
	else
		return (0);
	return (1);
}

void	ft_clear_exp(t_dblist **list, t_pars **exp, int pos)
{
	t_pars	*tmp;
	t_pars	*prev;
	t_pars	*next;

	tmp = *exp;
	prev = (*exp)->prev;
	next = (*exp)->next;
	while (pos-- > 0)
		tmp = tmp->next;
	if (prev == NULL)
	{
		(*list)->first = next;
		next->prev = NULL;
	}
	else
	{
		prev->next = next;
		next->prev = prev;
	}
	if ((*list)->last == *exp)
		(*list)->last = tmp;
	ft_free_pars(*exp);
	*exp = tmp;
}
