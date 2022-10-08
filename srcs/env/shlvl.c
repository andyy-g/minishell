/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 13:29:21 by agranger          #+#    #+#             */
/*   Updated: 2022/10/08 01:03:24 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	replace_env_var(t_env *env, char *newval)
{
	char	*newfull;
	int		size;

	if (!newval)
		return (0);
	size = ft_strlen(env->var) + ft_strlen(newval) + 2;
	newfull = ft_calloc(size, sizeof(char));
	if (!newfull)
	{
		ft_free(newval);
		return (0);
	}
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
