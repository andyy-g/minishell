/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 08:43:23 by charoua           #+#    #+#             */
/*   Updated: 2022/06/11 18:33:41 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free(t_dblist **list)
{
	t_pars	*tmp;
	t_pars	*to_free;

	to_free = (*list)->first;
	while (to_free)
	{
		tmp = to_free->next;
		if (to_free->str)
			free(to_free->str);
		if (to_free)
			free(to_free);
		to_free = tmp;
	}
}

t_pars	*ft_create_pars(t_pars *prev)
{
	t_pars	*pars;

	pars = (t_pars *)malloc(sizeof(t_pars));
	if (pars)
	{
		pars->str = NULL;
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

int	ft_parsing(char *str)
{
	t_pars		*pars;
	t_dblist	*list;

	list = (t_dblist *)malloc(sizeof(t_dblist));
	pars = ft_create_pars(NULL);
	if ((list) && (pars))
	{
		list->first = pars;
		list->last = pars;
		ft_add_pars(str, &pars, &list);
		ft_free(&list);
		free(list);
	}
	else
		return (0);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char	*tmp;

	(void) argc;
	(void) **argv;
	(void)envp;
	tmp = readline("");
	if (tmp)
	{
		ft_parsing(tmp);
		free(tmp);
	}
	return (0);
}
