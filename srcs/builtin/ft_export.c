/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 12:44:18 by charoua           #+#    #+#             */
/*   Updated: 2022/10/08 02:53:51 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_create_new_value(const char *str, int i, char *new)
{
	size_t	j;
	size_t	k;

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
				return (0);
			i++;
		}
		if (str[i] == '+' && str[i + 1] && str[i + 1] == '=')
			return (2);
	}
	if (str[i] == '!')
	{
		display_error(ERR_EXPORT_EVENT, str + i);
		return (-1);
	}
	else
		return (0);
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
		if (option == 1 || option == 2)
		{
			new = malloc(sizeof(t_env));
			if (new)
			{
				if (!ft_create_new(&new, node->cmd[i]))
				{
					ft_free(new);
					return (1);
				}
				tmp = env;
				if (option == 1 && ft_add_to_value(option, &tmp, &new) == 1)
					add_back_env(&env, new);
				if (option == 2 && ft_override_value(option, &tmp, &new) == 2)
					add_back_env(&env, new);
				ft_env_sort();
			}
		}
		else if (option != -1)
			ft_error_export(option, node->cmd[i]);
		i++;
	}
	return (1);
}
