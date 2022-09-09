/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 09:42:23 by charoua           #+#    #+#             */
/*   Updated: 2022/09/09 09:44:21 by charoua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_n(char *str)
{
	int	i;

	i = 0;
	if (str && str[i] && str[i] == '-')
	{
		i++;
		while (str[i] && str[i] == 'n')
			i++;
		if (i == (int)ft_strlen(str))
			return (1);
		else
			return (0);
	}
	else
		return (0);
}

int	ft_echo(t_node *node)
{
	int	i;
	int	nl;

	i = 1;
	nl = 0;
	if (node->cmd)
	{
		while (ft_check_n(node->cmd[i]))
		{
			nl = 1;
			i++;
		}
		while (node->cmd[i])
		{
			printf("%s", node->cmd[i]);
			if (node->cmd[i + 1])
				printf(" ");
			i++;
		}
		if (nl == 0)
			printf("\n");
	}
	return (1);
}
