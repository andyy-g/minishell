/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 14:34:49 by agranger          #+#    #+#             */
/*   Updated: 2022/09/12 16:27:29 by agranger         ###   ########.fr       */
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
	env->next = NULL;
	env->prev = NULL;
	return (env);
}

int	replace_env_var(t_env *env, char *newval)
{
	char	*newfull;
	int		size;

	if (!newval)
		return (0);
	size = ft_strlen(env->var) + ft_strlen(newval) + 2;
	newfull = ft_calloc(size, sizeof(char));
	if (!newfull)
		return (0);
	ft_strcpy(newfull, env->var);
	ft_strcat(newfull, "=");
	ft_strcat(newfull, newval);
	ft_free(env->full);
	env->full = newfull;
	ft_free(env->value);
	env->value = newval;
	return (1);
}

int	init_shlvl(t_env *shell)
{
	t_env	*shlvl;
	int		lvl;

	shlvl = shell;
	while (shell && ft_strcmp(shell->var, "SHELL"))
		shell = shell->next;
	if (!shell)
		return (1);
	if (!ft_strcmp(shell->value, "minishell"))
	{
		while (shlvl && ft_strcmp(shlvl->var, "SHLVL"))
			shlvl = shlvl->next;
		if (!shlvl)
			return (1);
		lvl = ft_atoi(shlvl->value) + 1;
		if (!replace_env_var(shlvl, ft_itoa(lvl)))
			return (0);
	}
	else
	{
		if (!replace_env_var(shell, ft_strdup("minishell")))
			return (0);
	}
	return (1);
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
