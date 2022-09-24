/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 10:27:51 by charoua           #+#    #+#             */
/*   Updated: 2022/09/24 10:28:28 by charoua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_node *node)
{
	t_env	*env;

	env = singleton_env(1, NULL, NULL);
	if (node->cmd[1])
	{
		printf("exit\nminishell: env: too many arguments\n");
		g_exit_status = 1;
	}
	while (env)
	{
		printf("%s\n", env->full);
		env = env->next;
	}
	return (1);
}
