/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_variable.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 15:48:08 by charoua           #+#    #+#             */
/*   Updated: 2022/09/26 17:00:29 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_ncmp(char *s1, char *s2, int n)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i] && n > 0)
	{
		i++;
		n--;
	}
	if (n == 0 && s2[i] == '\0')
		return (1);
	return (0);
}

char	*ft_replacebyvar(char *str, char *var, int start, int end)
{
	int		i;
	int		j;
	int		size;
	char	*new;

	i = 0;
	j = 0;
	size = ft_strlen(var) + ft_strlen(str) - end + start;
	new = (char *)malloc(sizeof(char) * size + 1);
	if (new)
	{
		while (str[i] && i < start)
		{
			new[i] = str[i];
			i++;
		}
		while (var[j])
			new[i++] = var[j++];
		j = 0;
		while (str[end + j])
			new[i++] = str[end + j++];
		new[i] = '\0';
	}
	free(str);
	return (new);
}

int	ft_check_variable(char **str, t_env *env, int i, int j)
{
	char	*exit_status;

	while (i - 1 > j && env)
	{
		if (env->var && ft_ncmp((*str) + j + 1, env->var, i - j - 1))
		{
			*str = ft_replacebyvar(*str, env->value, j, i);
			return (1);
		}
		env = env->next;
	}
	if (i == j + 2 && (*str)[i - 1] && (*str)[i - 1] == '?')
	{
		exit_status = ft_itoa(g_exit_status);
		*str = ft_replacebyvar(*str, exit_status, j, i);
		ft_free(exit_status);
		return (1);
	}
	return (0);
}

int	complete_str(t_pars **exp, int *j, int i)
{
	int	save;

	if (!*j && !(*exp)->str[i])
	{
		remove_pars(exp);
		*j = -1;
		return (1);
	}
	save = *j;
	while (*j < i && (*exp)->str[i])
		(*exp)->str[(*j)++] = (*exp)->str[i++];
	(*exp)->str[*j] = '\0';
	*j = save - 1;
	return (0);
}

int	ft_variable(t_pars **exp, t_env *env, int *j)
{
	int		i;
	int		found;
	char	*str;

	i = *j + 1;
	str = (*exp)->str;
	if (str[i] && (ft_isdigit(str[i]) || str[i] == '?'))
		i++;
	else
		while (str[i] && (ft_isalnum((int)str[i]) || str[i] == '_'))
			i++;
	found = ft_check_variable(&((*exp)->str), env, i, *j);
	if (found == 0)
	{
		if (complete_str(exp, j, i))
			return (1);
	}
	if (!(*exp)->str)
		return (0);
	return (1);
}
