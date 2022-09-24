/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 09:04:48 by charoua           #+#    #+#             */
/*   Updated: 2022/09/24 09:05:33 by charoua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	if (str)
	{
		while (str[i] != '\0')
		{
			if (str[i] < 48 || str[i] > 57)
				return (0);
			i++;
		}
	}
	return (1);
}

int	ft_exit(t_node *node)
{
	if (node->cmd[1] && node->cmd[2])
	{
		printf("exit\nminishell: exit: too many arguments\n");
		g_exit_status = 1;
		return (1);
	}
	else if (node->cmd[1])
	{
		if (!ft_isnum(node->cmd[1]))
		{
			printf("exit\nexit: %s: numeric argument required\n", node->cmd[1]);
			g_exit_status = 2;
		}
		else
		{
			printf("exit\n");
			g_exit_status = ft_atoi(node->cmd[1]);
		}
	}
	else
	{
		printf("exit\n");
		g_exit_status = 0;
	}
	return (0);
}
