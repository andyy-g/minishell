/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_specials_chars.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:25:04 by agranger          #+#    #+#             */
/*   Updated: 2022/09/22 14:32:28 by agranger         ###   ########.fr       */
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

int	ft_check_wildcard(t_pars **exp, t_dblist **list)
{
	int	i;

	i = 0;
	while ((*exp)->str[i] != '\0')
	{
		if ((*exp)->str[i] == '*'
			&& ((*exp)->sp_quote == 0 && (*exp)->db_quote == 0))
		{
			if (!ft_wildcard(list, exp))
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

int	ft_exp_quote(t_pars **exp, t_env **env, int i, char c)
{
	int	j;

	ft_clear((*exp)->str, i);
	while ((*exp)->str[i] && (*exp)->str[i] != c)
	{
		if ((*exp)->str[i] == '*')
			(*exp)->sp_quote = 1;
		if ((*exp)->str[i] == '$' && *env && c == 34 && (*exp)->str[i + 1]
			&& (ft_isalnum((*exp)->str[i + 1]) || (*exp)->str[i + 1] == '_'
				|| (*exp)->str[i + 1] == '?'))
		{
			j = i - 1;
			if (!ft_variable(&(*exp), *env, &i))
				return (-1);
			i = j;
		}
		i++;
	}
	ft_clear((*exp)->str, i);
	return (i);
}
