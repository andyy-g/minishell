/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 11:56:40 by charoua           #+#    #+#             */
/*   Updated: 2022/10/02 11:56:41 by charoua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env_pos(char *str, t_env *env)
{
	int		pos;

	pos = 1;
	while (env)
	{
		if (ft_strcmp(str, env->var) > 0)
			pos++;
		env = env->next;
	}
	return (pos);
}

t_env	*ft_env_sort(t_env *env)
{
	char	*str;
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		str = tmp->var;
		tmp->pos = ft_env_pos(str, env);
		tmp = tmp->next;
	}
	return (env);
}
