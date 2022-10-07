/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 12:44:18 by charoua           #+#    #+#             */
/*   Updated: 2022/10/06 23:52:37 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup_value(const char *str)
{
	char	*new;
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	while (str[i] != '\0' && str[i] != '=' && str[i] != '+')
		i++;
	if (str[i] == '=' || str[i] == '+')
	{
		if (str[i] == '+')
			i++;
		i++;
		j = 0;
		while (str[i + j] != '\0')
			j++;
		new = malloc(sizeof(char) * (j + 1));
		if (!new)
			return (NULL);
		k = 0;
		while (k < j)
		{
			new[k] = str[i + k];
			k++;
		}
		new[k] = '\0';
		return (new);
	}
	else
		return (NULL);
}

int	ft_syntax_export(char *str)
{
	int	i;

	i = 0;
	if (ft_isalpha((int)str[i]) || str[i] == '_')
	{
		i++;
		while (str[i] != '\0' && str[i] != '+' && str[i] != '=')
		{
			if (!ft_isalnum((int)str[i]) && str[i] != '_')
			{
				if (str[i] == '!')
				{
					display_error(ERR_EXPORT_EVENT, str + i);
					return (-1);
				}
				return (0);
			}
			i++;
		}
	}
	else
		return (0);
	if (str[i] == '+')
	{
		if (str[i + 1] && str[i + 1] == '=')
			return (2);
		return (0);
	}
	return (1);
}

int	ft_export(t_node *node)
{
	int		i;
	int		option;
	t_env	*env;
	t_env	*tmp;
	t_env	*new;

	i = 1;
	env = singleton_env(1, NULL, NULL);
	if (!(node->cmd[1]) && env)
		ft_print_env_exp(env);
	while (node->cmd[i])
	{
		option = ft_syntax_export(node->cmd[i]);
		if (option == 1)
		{
			new = malloc(sizeof(t_env));
			if (new)
			{
				ft_create_new(&new, node->cmd[i]);
				tmp = env;
				option = ft_add_to_value(option, &tmp, &new);
				if (option == 1)
					add_back_env(&env, new);
				ft_env_sort();
			}
		}
		else if (option == 2)
		{
			new = malloc(sizeof(t_env));
			if (new)
			{
				ft_create_new(&new, node->cmd[i]);
				tmp = env;
				option = ft_override_value(option, &tmp, &new);
				if (option == 2)
					add_back_env(&env, new);
				ft_env_sort();
			}
		}
		else
			ft_error_export(option, node->cmd[i]);
		i++;
	}
	return (1);
}
