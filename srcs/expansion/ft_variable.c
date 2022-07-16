/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_variable.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 15:48:08 by charoua           #+#    #+#             */
/*   Updated: 2022/07/16 15:30:17 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalnum(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') \
			|| (c >= '0' && c <= '9'));
}

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

char	*ft_replacebyvar(char *str, char *var, int size, int pos)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = (char *)malloc(sizeof(char) * size + 1);
	if (new)
	{
		while (var[i])
		{
			new[i] = var[i];
			i++;
		}
		while (str[pos + j])
		{
			new[i] = str[pos + j];
			j++;
			i++;
		}
		new[i] = '\0';
	}
	free(str);
	return (new);
}

int	ft_variable(t_pars **exp, t_env *env)
{
	int		i;
	int		size;
	char	*str;

	i = 1;
	str = (*exp)->str;
	while (str[i] && (ft_isalnum((int)str[i]) || str[i] == '_'))
		i++;
	while (i - 1 > 0 && env)
	{
		if (env->var && ft_ncmp(str + 1, env->var, i - 1))
		{
			size = ft_strlen(env->value) + ft_strlen(str) - i;
			(*exp)->str = ft_replacebyvar(str, env->value, size, i);
			if (!(*exp)->str)
				return (0);
			break ;
		}
		env = env->next;
	}
	return (1);
}
