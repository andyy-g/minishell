/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard_cpy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 14:26:43 by charoua           #+#    #+#             */
/*   Updated: 2022/07/16 16:04:51 by agranger         ###   ########.fr       */
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
		new->prev = (*prev);
		new->next = next;
		(*prev)->next = new;
		next->prev = new;
	}
	else
		return (0);
	return (1);
}
