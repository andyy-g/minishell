/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 14:18:28 by charoua           #+#    #+#             */
/*   Updated: 2022/07/16 16:03:04 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_hidden(char *str, char *dir)
{
	int	i;

	i = 0;
	if (str[i] && dir[i])
	{
		if (str[i] == '*' && dir[i] == '.')
			return (0);
	}
	return (1);
}

int	ft_compare(char *str, char *dir)
{
	int	i;
	int	j;

	i = 0;
	if (!str[i] && !dir[i])
		return (1);
	if (str[i] && dir[i] && str[i] == dir[i])
		return (ft_compare(str + i + 1, dir + i + 1));
	if (str[i] != dir[i] && str[i] == '*')
	{
		if (!(str[i + 1]))
			return (1);
		else
		{
			j = 0;
			while (dir[i + j])
			{
				if (ft_compare(str + i + 1, dir + i + j) == 1)
					return (1);
				j++;
			}
			return (0);
		}
	}
	return (0);
}

int	ft_find_pos(t_pars **exp, char *dir, int pos)
{
	int i;
	int	j;
	t_pars	*tmp;

	i = 0;
	tmp = (*exp)->next;
	while (i < pos)
	{
		j = 0;
		while (tmp->str[j] != '\0' && dir[j] != '\0' && tmp->str[j] == dir[j])
		{
			j++;
		}
		if (dir[j] < tmp->str[j])
			return (i);
		tmp = tmp->next;
		i++;
	}
	return (i - 1);
}

int	ft_add_wild(t_pars **exp, char *dir, int pos)
{
	int i;
	t_pars	*tmp;

	i = 0;
	tmp = *exp;
	while (i < pos)
	{
		tmp = tmp->next;
		i++;
	}
	if (!ft_copy_pars(&tmp, dir))
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
	if (*exp && (*exp)->str)
		free((*exp)->str);
	if (*exp)
		free(*exp);
}

int	ft_wildcard(t_dblist **list, t_pars **exp)
{
	int				found;
	DIR				*d;
	struct dirent	*dir;
	(void)list;

	found = 0;
	d = opendir(".");
	if (d)
	{
		dir = readdir(d);
		while (dir)
		{
			if (ft_compare((*exp)->str, dir->d_name) && ft_check_hidden((*exp)->str, dir->d_name))
			{
				if (!ft_add_wild(exp, dir->d_name, ft_find_pos(exp, dir->d_name, found)))
					return (0);
				found ++;
			}
			dir = readdir(d);
		}
		closedir(d);
	}
	if (found > 0)
		ft_clear_exp(list, exp, found);
	return (1);
}
