/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 22:35:11 by agranger          #+#    #+#             */
/*   Updated: 2022/07/20 23:07:26 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_size_env(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

char	**env_to_str_arr(t_env *env)
{
	char	**ret;
	int		i;
	int		size;

	size = get_size_env(env);
	ret = malloc(sizeof(char *) * (size + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (env)
	{
		ret[i++] = env->full;
		env = env->next;
	}
	ret[i] = NULL;
	return (ret);
}
