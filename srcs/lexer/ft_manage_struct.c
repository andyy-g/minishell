/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_manage_struct.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 17:56:07 by charoua           #+#    #+#             */
/*   Updated: 2022/06/23 15:33:20 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tokens(t_dblist *list)
{
	t_pars	*tmp;
	t_pars	*to_free;

	to_free = list->first;
	while (to_free)
	{
		tmp = to_free->next;
		if (to_free->str)
			free(to_free->str);
		if (to_free)
			free(to_free);
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
