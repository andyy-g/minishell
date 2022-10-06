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

int	ft_env_pos(char *str)
{
	int		pos;
	t_env	*env;

	env = singleton_env(1, NULL, NULL);
	pos = 1;
	while (env)
	{
		if (ft_strcmp(str, env->var) > 0)
			pos++;
		env = env->next;
	}
	return (pos);
}

void	ft_env_sort(void)
{
	char	*str;
	t_env	*env;

	env = singleton_env(1, NULL, NULL);
	while (env)
	{
		str = env->var;
		env->pos = ft_env_pos(str);
		env = env->next;
	}
}
