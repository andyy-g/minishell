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
}

int	ft_unset(t_node *node)
{
	int		i;
	int		size;
	t_env	*env;

	i = 1;
	while (node->cmd[i])
	{
		env = singleton_env(1, NULL, NULL);
		while (env)
		{
			size = ft_strlen(node->cmd[i]);
			if (env->var && ft_ncmp(node->cmd[i], env->var, size))
			{
				ft_unset_var(&env);
				break ;
			}
			env = env->next;
		}
		i++;
	}
	return (1);
}
