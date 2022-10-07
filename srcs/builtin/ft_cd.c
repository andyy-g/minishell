/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 14:22:22 by charoua           #+#    #+#             */
/*   Updated: 2022/10/07 14:23:03 by charoua          ###   ########.fr       */
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

int	ft_create_pwd(const char *var, char *cwd)
{
	char	*cwd;
	char	*pwd;
	t_env	*env;
	t_env	*tmp;
	t_env	*new;

	env = singleton_env(1, NULL, NULL);
	tmp = env;
	cwd = (char *)malloc(sizeof(char) * PATH_MAX);
	if (!cwd || getcwd(cwd, PATH_MAX) == NULL)
		return (0);
	while (tmp)
	{
		if (tmp->var && ft_strcmp(var, tmp->var) == 0)
		{
			pwd = tmp->value;
			tmp->value = ft_strdup(cwd);
			free(pwd);
			pwd = tmp->full;
			tmp->full = ft_strdup_two(tmp->var, tmp->value);
			free(pwd);
			free(cwd);
			ft_env_sort();
			return (1);
		}
		tmp = tmp->next;
	}
	if (option == 1)
	{
		new = malloc(sizeof(t_env));
		if (new)
		{
			new->var = ft_strdup(var);
			new->value = ft_strdup(cwd);
			new->full = ft_strdup_two(new->var, new->value);
			new->pos = 0;
			new->prev = NULL;
			new->next = NULL;
			free(cwd);
			add_back_env(&env, new);
			ft_env_sort();
			return (1);
		}
	}
	return (0);
}

int	ft_update_pwd(const char *var, int option)
{
	char	*cwd;
	char	*pwd;
	t_env	*env;
	t_env	*tmp;
	t_env	*new;

	env = singleton_env(1, NULL, NULL);
	tmp = env;
	cwd = (char *)malloc(sizeof(char) * PATH_MAX);
	if (!cwd || getcwd(cwd, PATH_MAX) == NULL)
		return (0);
	while (tmp)
	{
		if (tmp->var && ft_strcmp(var, tmp->var) == 0)
		{
			pwd = tmp->value;
			tmp->value = ft_strdup(cwd);
			free(pwd);
			pwd = tmp->full;
			tmp->full = ft_strdup_two(tmp->var, tmp->value);
			free(pwd);
			free(cwd);
			ft_env_sort();
			return (1);
		}
		tmp = tmp->next;
	}
	if (option == 1)
	{
		new = malloc(sizeof(t_env));
		if (new)
		{
			new->var = ft_strdup(var);
			new->value = ft_strdup(cwd);
			new->full = ft_strdup_two(new->var, new->value);
			new->pos = 0;
			new->prev = NULL;
			new->next = NULL;
			free(cwd);
			add_back_env(&env, new);
			ft_env_sort();
			return (1);
		}
	}
	return (0);
}

int	ft_go_path(int option)
{
	t_env	*env;
	char	*env_path;

	env_path = NULL;
	env = singleton_env(1, NULL, NULL);
	if (option == 0)
	{
		ft_update_pwd("OLDPWD", 0);
		env_path = get_env_path(env, "HOME");
		if (!env_path)
		{
			printf("minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else if (option == 1)
	{
		env_path = get_env_path(env, "OLDPWD");
		if (!env_path)
		{
			printf("minishell: cd: OLDPWD not set\n");
			return (1);
		}
		else
		{
			printf("%s\n", env_path);
			ft_update_pwd("OLDPWD", 1);
		}
	}
	g_exit_status = chdir(env_path);
	free(env_path);
	return (-g_exit_status);
}

int	ft_cd(t_node *node)
{
	if (node && node->cmd[1] && node->cmd[2])
		display_error(ERR_CD_NBARG, node->cmd[1]);
	else
	{
		if (!(node->cmd[1]))
			g_exit_status = ft_go_path(0);
		else if (node->cmd[1] && ft_strcmp(node->cmd[1], "-") == 0)
			g_exit_status = ft_go_path(1);
		else
		{
			ft_update_pwd("OLDPWD", 1);
			g_exit_status = -chdir(node->cmd[1]);
			ft_update_pwd("PWD", 1);
			if (g_exit_status != 0)
				display_error(ERR_CD_CHDIR, node->cmd[1]);
		}
		if (g_exit_status == 0)
			ft_update_pwd("PWD", 1);
	}
	return (1);
}
