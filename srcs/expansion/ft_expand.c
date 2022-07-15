/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 12:57:20 by charoua           #+#    #+#             */
/*   Updated: 2022/07/14 14:17:59 by charoua          ###   ########.fr       */
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

int	ft_expand(t_dblist **list, t_env **env)
{
	int		i;
	t_pars	*exp;
	t_pars	*tmp;

	exp = (*list)->first;
	while (exp->next)
	{
		i = 0;
		tmp = exp->next;
		if (exp->str[i] == '$' && exp->sp_quote == 0 && *env)
			ft_variable(&exp, *env);
		else if (exp->str[i] == '~' && *env)
			ft_home(&exp, *env);
		while (exp->str[i] != '\0')
		{
			if (exp->str[i] == '*' && exp->sp_quote == 0 && exp->db_quote == 0)
			{
				ft_wildcard(list, &exp);
				break ;
			}
			i++;
		}
		exp = tmp;
	}
	return (1);
}
