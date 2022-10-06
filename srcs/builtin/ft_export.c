/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 12:44:18 by charoua           #+#    #+#             */
/*   Updated: 2022/10/06 23:52:37 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_envsize(t_env *env)
{
	int		i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

int	ft_export(t_node *node)
{
	int		i;
	int		size;
	t_env	*env;
	t_env	*tmp;
	t_env	*test;

	i = 1;
	env = singleton_env(1, NULL, NULL);
	size = ft_envsize(env);
	if (!(node->cmd[1]) && env)
	{
		while (i <= size)
		{
			tmp = env;
			while (tmp && i != tmp->pos)
				tmp = tmp->next;
			printf("declare -x %s=\"%s\"\n", tmp->var, tmp->value);
			i++;
		}
	}
	else if ((node->cmd[1]))
	{
		if (!ft_strcmp(node->cmd[1], "test"))
		{
			printf("Hello\n");
			test = new_env("TEST=test");
			if (!test)
				return (0);
			add_back_env(&env, test);
		}
	}
	//si pas d'env pour env afficher PWD SHLVL et _, pour export OLDPWD vide PWD et SHLVL
	return (1);
}
