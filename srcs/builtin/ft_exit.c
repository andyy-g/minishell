/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 09:04:48 by charoua           #+#    #+#             */
/*   Updated: 2022/10/08 18:38:16 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_length(char *str, int *i)
{
	if (str[0] == '-' || str[0] == '+')
	{
		if (ft_strlen(str) > 20)
			return (1);
		(*i)++;
	}
	else
	{
		if (ft_strlen(str) > 19)
			return (1);
	}
	return (0);
}

int	check_each_figure(char *str, int pos, int i)
{
	int	j;

	j = 0;
	while (str[i])
	{
		if (!str[i + 1])
		{
			if (pos)
				return (str[i] > '7');
			return (str[i] > '8');
		}
		else if (str[i] > LL_LIMIT[j])
			return (1);
		i++;
		j++;
	}
	return (1);
}

bool	is_too_high_or_low(char *str)
{
	int	pos;
	int	i;

	pos = 1;
	i = 0;
	if (check_length(str, &i))
		return (true);
	if (ft_strlen(&str[i]) != 19)
		return (false);
	if (i != 0 && str[i - 1] == '-')
		pos = 0;
	if (check_each_figure(str, pos, i))
		return (true);
	return (false);
}

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	if (str)
	{
		if (str[i] == '-' || str[i] == '+')
			i++;
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
	if (node && !node->parent)
		printf("exit\n");
	if (node && node->cmd[1] && node->cmd[2])
	{
		if (!ft_isnum(node->cmd[1]) || is_too_high_or_low(node->cmd[1]))
			display_error(ERR_EXIT_ARG_NO_NUM, node->cmd[1]);
		else
		{
			display_error(ERR_EXIT_NBARG, node->cmd[1]);
			return (1);
		}
	}
	else if (node && node->cmd[1])
	{
		if (!ft_isnum(node->cmd[1]) || is_too_high_or_low(node->cmd[1]))
			display_error(ERR_EXIT_ARG_NO_NUM, node->cmd[1]);
		else
			g_exit_status = ft_atoi(node->cmd[1]);
	}
	exit_child_builtin(node);
	return (0);
}
