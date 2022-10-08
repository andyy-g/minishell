/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 10:27:51 by charoua           #+#    #+#             */
/*   Updated: 2022/10/08 03:17:23 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_node *node)
{
	t_env	*env;

	env = singleton_env(1, NULL, NULL);
	if (node->cmd[1])
		display_error(ERR_ENV_NBARG, node->cmd[1]);
	else
	{
		while (env)
		{
			if (env->full && env->value[0])
				printf("%s\n", env->full);
			env = env->next;
		}
	}
	g_exit_status = 0;
	return (1);
}
