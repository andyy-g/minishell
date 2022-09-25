/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 09:04:48 by charoua           #+#    #+#             */
/*   Updated: 2022/09/24 14:43:36 by agranger         ###   ########.fr       */
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
	printf("exit\n");
	if (node->cmd[1] && node->cmd[2])
	{
		if (!ft_isnum(node->cmd[1]))
			display_error(ERR_EXIT_ARG_NO_NUM, node->cmd[1]);
		else
		{
			display_error(ERR_EXIT_NBARG, node->cmd[1]);
			return (1);
		}
	}
	else if (node->cmd[1])
	{
		if (!ft_isnum(node->cmd[1]))
			display_error(ERR_EXIT_ARG_NO_NUM, node->cmd[1]);
		else
			display_error(ERR_EXIT_ARG_NUM, node->cmd[1]);
	}
	else
		g_exit_status = 0;
	exit_minishell(node);
	return (0);
}
