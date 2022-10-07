/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 20:45:36 by charoua           #+#    #+#             */
/*   Updated: 2022/10/07 23:12:58 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup_two(char *var, char *pwd)
{
	int		i;
	int		j;
	char	*full;

	if (!var || !pwd)
		return (NULL);
	full = malloc(sizeof(char) * (ft_strlen(var) + ft_strlen(pwd) + 2));
	if (!full)
		return (NULL);
	i = 0;
	while (var[i])
	{
		full[i] = var[i];
		i++;
	}
	full[i++] = '=';
	j = 0;
	while (pwd[j])
	{
		full[i + j] = pwd[j];
		j++;
	}
	full[i + j] = '\0';
	return (full);
}

char	*get_env_path(t_env *env, const char *var)
{
	while (env)
	{
		if (env->var && ft_strcmp(var, env->var) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

void	ft_create_pwd(const char *var, char *cwd)
{
	t_env	*env;
	t_env	*new;

	env = singleton_env(1, NULL, NULL);
	new = malloc(sizeof(t_env));
	if (new)
	{
		new->var = ft_strdup(var);
		new->value = ft_strdup(cwd);
		new->full = ft_strdup_two(new->var, new->value);
		new->pos = 0;
		new->prev = NULL;
		new->next = NULL;
		add_back_env(&env, new);
		ft_env_sort();
	}
}

void	ft_update_pwd(const char *var, int option)
{
	char	*cwd;
	t_env	*env;
	t_env	*tmp;

	env = singleton_env(1, NULL, NULL);
	tmp = env;
	cwd = (char *)malloc(sizeof(char) * PATH_MAX);
	if (cwd && getcwd(cwd, PATH_MAX))
	{
		while (tmp)
		{
			if (tmp->var && ft_strcmp(var, tmp->var) == 0)
			{
				free(tmp->value);
				tmp->value = ft_strdup(cwd);
				free(tmp->full);
				tmp->full = ft_strdup_two(tmp->var, tmp->value);
				ft_env_sort();
			}
			tmp = tmp->next;
		}
		if (option == 1 && !get_env_path(env, var))
			ft_create_pwd(var, cwd);
	}
	free(cwd);
}
