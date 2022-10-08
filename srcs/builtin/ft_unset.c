/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 11:04:36 by charoua           #+#    #+#             */
/*   Updated: 2022/10/08 02:00:18 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset_var(t_env *env)
{
	t_env	*prev;
	t_env	*next;

	prev = env->prev;
	next = env->next;
	env->next = NULL;
	free_env(env);
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	ft_env_sort();
	return (1);
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
			if (!ft_isalnum((int)str[i]) && str[i] != '_')
				return (0);
			i++;
		}
	}
	else
		return (0);
	return (1);
}

int	ft_find_var(t_env **env, char *str, int size)
{
	while (*env)
	{
		if ((*env)->var && ft_ncmp(str, (*env)->var, size))
		{
			if (!ft_unset_var(*env))
				return (0);
			break ;
		}
		*env = (*env)->next;
	}
	return (1);
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
			if (!ft_find_var(&env, node->cmd[i], ft_strlen(node->cmd[i])))
				return (0);
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
