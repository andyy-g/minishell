/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 11:04:36 by charoua           #+#    #+#             */
/*   Updated: 2022/09/24 13:10:19 by charoua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_unset_var(t_env **env)
{
	char	*tmp;
	t_env	*prev;

	while ((*env)->next)
	{
		tmp = ft_strdup(((*env)->next)->var);
		ft_free((*env)->var);
		(*env)->var = tmp;
		tmp = ft_strdup(((*env)->next)->value);
		ft_free((*env)->value);
		(*env)->value = tmp;
		tmp = ft_strdup(((*env)->next)->full);
		ft_free((*env)->full);
		(*env)->full = tmp;
		*env = (*env)->next;
	}
	prev = (*env)->prev;
	prev->next = NULL;
	ft_free((*env)->var);
	ft_free((*env)->value);
	ft_free((*env)->full);
	ft_free((*env));
	ft_env_sort();
}

int	ft_syntax_unset(char *str)
{
	int	i;

	i = 0;
	if (ft_isalpha((int)str[i]) || str[i] == '_')
	{
		i++;
		while (str[i] != '\0')
		{
			if (!ft_isalnum((int)str[i]))
				return (0);
			i++;
		}
	}
	else
		return (0);
	return (1);
}

void	ft_find_var(t_env **env, char *str, int size)
{
	while (*env)
	{
		if ((*env)->var && ft_ncmp(str, (*env)->var, size))
		{
			ft_unset_var(&(*env));
			break ;
		}
		*env = (*env)->next;
	}
}

int	ft_unset(t_node *node)
{
	int		i;
	int		err;
	t_env	*env;

	i = 1;
	err = 0;
	while (node->cmd[i])
	{
		env = singleton_env(1, NULL, NULL);
		if (ft_syntax_unset(node->cmd[i]))
		{
			ft_find_var(&env, node->cmd[i], ft_strlen(node->cmd[i]));
			if (err == 0)
				g_exit_status = 0;
		}
		else
		{
			display_error(ERR_UNSET_ARG, node->cmd[1]);
			err = 1;
		}
		i++;
	}
	return (1);
}
