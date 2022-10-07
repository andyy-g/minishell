/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 09:05:09 by charoua           #+#    #+#             */
/*   Updated: 2022/09/24 10:59:57 by charoua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void	ft_switch_pwd(t_env **env, t_env **pwd, t_env **oldpwd, char *cwd)
{
	char *tmp;

	if ((*pwd) && (*oldpwd))
	{
		printf("\n---------1------------\n");
		tmp = (*oldpwd)->value;
		(*oldpwd)->value = ft_strdup((*pwd)->value);
		free(tmp);
		tmp = (*oldpwd)->full;
		(*oldpwd)->full = ft_strdup_two((*oldpwd)->var, (*oldpwd)->value);
		free(tmp);

		tmp = (*pwd)->value;
		(*pwd)->value = ft_strdup(cwd);
		free(tmp);
		tmp = (*pwd)->full;
		(*pwd)->full = ft_strdup_two((*pwd)->var, cwd);
		free(tmp);
	}
	else if ((*pwd) && !(*oldpwd))
	{
		printf("\n---------2------------\n");
		(*oldpwd) = malloc(sizeof(t_env));
		(*oldpwd)->var = ft_strdup("OLDPWD");
		(*oldpwd)->value = ft_strdup((*pwd)->value);
		(*oldpwd)->full = ft_strdup_two((*oldpwd)->var, (*oldpwd)->value);
		(*oldpwd)->prev = (*env);
		(*oldpwd)->next = NULL;
		(*env)->next = (*oldpwd);
		tmp = (*pwd)->value;
		(*pwd)->value = ft_strdup(cwd);
		free(tmp);
		tmp = (*pwd)->full;
		(*pwd)->full = ft_strdup_two((*pwd)->var, cwd);
		free(tmp);
	}
	else if (!(*pwd))
	{
		printf("\n---------3------------\n");
		(*pwd) = malloc(sizeof(t_env));
		(*pwd)->var = ft_strdup("PWD");
		(*pwd)->value = ft_strdup(cwd);
		(*pwd)->full = ft_strdup_two((*pwd)->var, cwd);
		(*pwd)->prev = (*env);
		(*pwd)->next = NULL;
		(*env)->next = (*pwd);
	}
}

void	ft_change_env_pwd(char *cwd)
{
	t_env	*pwd;
	t_env	*oldpwd;
	t_env	*env;

	pwd = NULL;
	oldpwd = NULL;
	env = singleton_env(1, NULL, NULL);
	while (env)
	{
		if (env->var && ft_ncmp("PWD", env->var, 3))
			pwd = env;
		if (env->var && ft_ncmp("OLDPWD", env->var, 6))
			oldpwd = env;
		env = env->next;
	}
	env = singleton_env(1, NULL, NULL);
	while (env && env->next)
		env = env->next;
	ft_switch_pwd(&env, &pwd, &oldpwd, cwd);
	env = singleton_env(1, NULL, NULL);
	ft_env_sort();
}

int		ft_pwd(t_node *node)
{
	char	*cwd;

	(void)node;
	cwd = (char *)malloc(sizeof(char) * PATH_MAX);
	if (cwd && getcwd(cwd, PATH_MAX))
	{
		printf("%s\n", cwd);
		ft_change_env_pwd(cwd);
		free(cwd);
		g_exit_status = 0;
	}
	else
		g_exit_status = 1;
	return (1);
}
