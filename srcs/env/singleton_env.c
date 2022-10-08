/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 14:34:49 by agranger          #+#    #+#             */
/*   Updated: 2022/10/08 10:45:23 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_back_env(t_env **first, t_env *new)
{
	t_env	*tmp;

	tmp = *first;
	if (!*first)
	{
		*first = new;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

t_env	*error_malloc(t_env *env)
{
	free_env(env);
	return (NULL);
}

t_env	*new_env(char *str)
{
	t_env	*env;
	int		equal;

	equal = 0;
	while (str[equal] != '=')
		equal++;
	env = malloc(sizeof(*env));
	if (!env)
		return (NULL);
	env->full = ft_strdup(str);
	if (!env->full)
		return (error_malloc(env));
	env->var = ft_strndup(str, equal);
	if (!env->var)
		return (error_malloc(env));
	env->value = ft_strdup(&str[equal + 1]);
	if (!env->value)
		return (error_malloc(env));
	env->pos = 0;
	env->next = NULL;
	env->prev = NULL;
	return (env);
}

t_env	*create_env(char **envp, int *status)
{
	t_env	*env;
	t_env	*tmp;

	env = NULL;
	while (*envp)
	{
		tmp = new_env(*envp);
		if (!tmp)
		{
			free_env(env);
			*status = 0;
			return (NULL);
		}
		add_back_env(&env, tmp);
		envp++;
	}
	if (!init_shlvl(env))
	{
		free_env(env);
		return (NULL);
	}
	return (env);
}

t_env	*singleton_env(int i, int *status, char **envp)
{
	static t_env	*env;

	if (i == 0)
	{
		if (envp)
		{
			env = create_env(envp, status);
			ft_env_sort();
			return (env);
		}
		else
		{
			env = NULL;
			return (env);
		}
	}
	if (i == 1)
		return (env);
	if (i == 2)
		free_env(env);
	return (NULL);
}
