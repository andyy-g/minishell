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
		printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		return (1);// sortie erreur, $? = 1;
	}
	else if (node->cmd[1])
	{
		if (!ft_isnum(node->cmd[1]))
		{
			printf("exit\n");
			printf("exit: %s: numeric argument required", node->cmd[1]);// sortie erreur, $? = 2;
		}
		else
			printf("exit\n");// changer valeur $? par ft_atoi(node->cmd[1]);
	}
	else
		printf("exit\n");// changer valeur $? par 0;
	return (0);
}

// Comment tu sais que tu dois envoyer sur sortie d'erreur
