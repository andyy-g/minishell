/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 14:34:49 by agranger          #+#    #+#             */
/*   Updated: 2022/06/23 18:55:59 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		ft_free(env->var);
		ft_free(env->value);
		ft_free(env->full);
		ft_free(env);
		env = tmp;
	}
}

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
	{
		free_env(env);
		return (NULL);
	}
	env->var = ft_strndup(str, equal);
	if (!env->var)
	{
		free_env(env);
		return (NULL);
	}
	env->value = ft_strdup(&str[equal + 1]);
	if (!env->value)
	{
		free_env(env);
		return (NULL);
	}
	env->next = NULL;
	env->prev = NULL;
	return (env);
}

t_env	*create_env(char **envp)
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
			return (NULL);
		}
		add_back_env(&env, tmp);
		envp++;
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
			env = create_env(envp);
			if (!env && *status)
				*status = 0;
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
