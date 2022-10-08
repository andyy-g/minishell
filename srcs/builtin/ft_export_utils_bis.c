/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils_bis.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 22:03:34 by charoua           #+#    #+#             */
/*   Updated: 2022/10/08 02:44:55 by agranger         ###   ########.fr       */
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

char	*ft_strdup_two_val(char *var, char *new)
{
	int		i;
	int		j;
	char	*full;

	if (!var && !new)
		return (NULL);
	full = malloc(sizeof(char) * (ft_strlen(var) + ft_strlen(new) + 2));
	if (!full)
		return (NULL);
	i = 0;
	while (var && var[i])
	{
		full[i] = var[i];
		i++;
	}
	j = 0;
	while (new && new[j])
	{
		full[i + j] = new[j];
		j++;
	}
	full[i + j] = '\0';
	return (full);
}

char	*ft_strdup_var(const char *str)
{
	char	*new;
	size_t	i;
	size_t	j;

	i = 0;
	while (str && str[i] != '\0' && str[i] != '+' && str[i] != '=')
		i++;
	new = malloc(sizeof(char) * (i + 1));
	if (!new)
		return (NULL);
	j = 0;
	while (j < i)
	{
		new[j] = str[j];
		j++;
	}
	new[j] = '\0';
	return (new);
}

char	*ft_strdup_value(const char *str)
{
	char	*new;
	size_t	i;

	i = 0;
	new = NULL;
	if (str)
	{
		while (str[i] != '\0' && str[i] != '=' && str[i] != '+')
			i++;
		if (str[i] == '=' || str[i] == '+')
			new = ft_create_new_value(str, i, new);
		else
		{
			new = ft_strdup("");
			if (!new)
				return (NULL);
		}
	}
	return (new);
}

char	*ft_strdup_full(char *var, char *value)
{
	char	*new;
	size_t	i;
	size_t	j;

	i = 0;
	new = malloc(sizeof(char) * (ft_strlen(var) + ft_strlen(value) + 2));
	if (!new)
		return (NULL);
	while (var && var[i] != '\0')
	{
		new[i] = var[i];
		i++;
	}
	j = 0;
	if (value)
	{
		new[i++] = '=';
		while (value[j] != '\0')
		{
			new[i + j] = value[j];
			j++;
		}
	}
	new[i + j] = '\0';
	return (new);
}
