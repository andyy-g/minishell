/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 12:57:20 by charoua           #+#    #+#             */
/*   Updated: 2022/07/16 15:54:38 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_home(t_pars **exp, t_env *env)
{
	int		size;
	char	*str;

	str = (*exp)->str;
	while (env)
	{
		if (env->var && ft_ncmp("HOME", env->var, 4))
		{
			size = ft_strlen(env->value) + ft_strlen(str) - 1;
			(*exp)->str = ft_replacebyvar(str, env->value, size, 1);
			break ;
		}
		env = env->next;
	}
}

int	ft_check_wildcard(t_pars *exp, t_dblist **list)
{
	int	i;

	i = 0;
	while (exp->str[i] != '\0')
	{
		if (exp->str[i] == '*' && exp->sp_quote == 0 && exp->db_quote == 0)
		{
			if (!ft_wildcard(list, &exp))
				return (0);
			break ;
		}
		i++;
	}
	return (1);
}

void	ft_clear(char *str, int i)
{
	while (str[i] != '\0')
	{
		str[i] = str[i + 1];
		i++;
	}
}

int	ft_expand(t_dblist **list, t_env **env)
{
	t_pars	*exp;
	t_pars	*tmp;
	int		i;

	exp = (*list)->first;
	while (exp->next)
	{
		i = 0;
		tmp = exp->next;
		if (exp->str[i] == '~' && !(exp->str[i + 1]) && *env)
			ft_home(&exp, *env);
		while (exp->str[i] != '\0')
		{
			if (exp->str[i] == 39)
			{
				ft_clear(exp->str, i);
				i++;
				while (exp->str[i] && exp->str[i] != 39)
				{
					if (exp->str[i] == '*')
						exp->sp_quote = 1;
					i++;
				}
				ft_clear(exp->str, i);
				if (i >= 1)
					i--;
			}
			if (exp->str[i] == 34)
			{
				ft_clear(exp->str, i);
				i++;
				while (exp->str[i] && exp->str[i] != 34)
				{
					if (exp->str[i] == '$' && *env)
					{
						if (!ft_variable(&exp, *env, i))
							return (0);
					}
					i++;
				}
				ft_clear(exp->str, i);
				if (i >= 1)
					i--;
			}
			if (exp->str[i] == '$' && *env)
			{
				if (!ft_variable(&exp, *env, i))
					return (0);
			}
			i++;
		}
		if (!ft_check_wildcard(exp, list))
			return (0);
		exp = tmp;
	}
	return (1);
}
