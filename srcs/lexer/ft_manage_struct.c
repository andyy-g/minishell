/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_manage_struct.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 17:56:07 by charoua           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/06/23 15:33:20 by agranger         ###   ########.fr       */
=======
/*   Updated: 2022/06/13 18:03:05 by charoua          ###   ########.fr       */
>>>>>>> 26dc0cbfb5e4a8f6a76d82557e8b49c79a15c8ba
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

<<<<<<< HEAD
void	ft_free_tokens(t_dblist *list)
=======
void	ft_free(t_dblist **list)
>>>>>>> 26dc0cbfb5e4a8f6a76d82557e8b49c79a15c8ba
{
	t_pars	*tmp;
	t_pars	*to_free;

<<<<<<< HEAD
	to_free = list->first;
=======
	to_free = (*list)->first;
>>>>>>> 26dc0cbfb5e4a8f6a76d82557e8b49c79a15c8ba
	while (to_free)
	{
		tmp = to_free->next;
		if (to_free->str)
			free(to_free->str);
		if (to_free)
			free(to_free);
		to_free = tmp;
	}
<<<<<<< HEAD
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
=======
>>>>>>> 26dc0cbfb5e4a8f6a76d82557e8b49c79a15c8ba
}

t_pars	*ft_create_pars(t_pars *prev)
{
	t_pars	*pars;

	pars = (t_pars *)malloc(sizeof(t_pars));
	if (pars)
	{
		pars->str = NULL;
<<<<<<< HEAD
		pars->token = -1;
=======
>>>>>>> 26dc0cbfb5e4a8f6a76d82557e8b49c79a15c8ba
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
