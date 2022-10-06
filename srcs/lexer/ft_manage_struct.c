/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_manage_struct.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 17:56:07 by charoua           #+#    #+#             */
/*   Updated: 2022/10/06 11:20:43 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_pars(t_pars *token)
{
	if (!token)
		return ;
	ft_free(token->str);
	ft_free(token);
}

void	ft_free_tokens(t_dblist *list)
{
	t_pars	*tmp;
	t_pars	*to_free;

	to_free = list->first;
	while (to_free && to_free->prev)
		to_free = to_free->prev;
	while (to_free)
	{
		tmp = to_free->next;
		ft_free_pars(to_free);
		to_free = tmp;
	}
	free(list);
}

t_dblist	*create_list(void)
{
	t_dblist	*list;

	list = malloc(sizeof(*list));
	if (!list)
		return (NULL);
	list->first = NULL;
	list->last = NULL;
	list->curr = NULL;
	return (list);
}

t_pars	*ft_create_pars(t_pars *prev)
{
	t_pars	*pars;

	pars = (t_pars *)malloc(sizeof(t_pars));
	if (pars)
	{
		pars->str = NULL;
		pars->token = -1;
		pars->db_quote = 0;
		pars->sp_quote = 0;
		pars->heredoc = NULL;
		if (prev)
			pars->prev = prev;
		else
			pars->prev = NULL;
		pars->next = NULL;
	}
	else
		return (NULL);
	return (pars);
}

void	remove_pars(t_pars **token)
{
	t_pars	*prev;
	t_pars	*next;

	prev = (*token)->prev;
	next = (*token)->next;
	ft_free_pars(*token);
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	*token = next;
}
