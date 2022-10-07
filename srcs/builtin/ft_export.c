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

char	*ft_strdup_full(char *var, char *value)
{
	char	*new;
	size_t	i;
	size_t	j;

	i = 0;
	new = malloc(sizeof(char) * (ft_strlen(var) + ft_strlen(value) + 1));
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

int	ft_add_to_value(int option, t_env **tmp, t_env **new)
{
	while (*tmp)
	{
		if ((*tmp)->var && ft_strcmp((*new)->var, (*tmp)->var) == 0)
		{
			free((*new)->var);
			free((*tmp)->value);
			(*tmp)->value = ft_strdup((*new)->value);
			free((*new)->value);
			free((*tmp)->full);
			(*tmp)->full = ft_strdup((*new)->full);
			free((*new)->full);
			free(*new);
			option = 3 ;
			break;
		}
		(*tmp) = (*tmp)->next;
	}
	return (option);
}

int	ft_override_value(int option, t_env **tmp, t_env **new)
{
	char	*delete;

	while (*tmp)
	{
		if ((*tmp)->var && ft_strcmp((*new)->var, (*tmp)->var) == 0)
		{
			free((*new)->var);
			delete = (*tmp)->value;
			(*tmp)->value = ft_strdup_two_val((*tmp)->value, (*new)->value);
			free(delete);
			free((*new)->value);
			free((*tmp)->full);
			(*tmp)->full = ft_strdup_full((*tmp)->var, (*tmp)->value);
			free((*new)->full);
			free(*new);
			option = 3 ;
			break;
		}
		(*tmp) = (*tmp)->next;
	}
	return (option);
}

void	ft_print_env_exp(t_env *env)
{
	int		i;
	int		size;
	t_env	*tmp;

	i = 1;
	size = ft_envsize(env);
	while (i <= size)
	{
		tmp = env;
		while (tmp && i != tmp->pos)
			tmp = tmp->next;
		printf("declare -x %s", tmp->var);
		if (tmp->value)
			printf("=\"%s\"\n", tmp->value);
		else
			printf("\n");
		i++;
	}
}

void	ft_create_new(t_env	**new, char *str)
{
	(*new)->var = ft_strdup_var(str);
	(*new)->value = ft_strdup_value(str);
	(*new)->full = ft_strdup_full((*new)->var, (*new)->value);
	(*new)->pos = 0;
	(*new)->prev = NULL;
	(*new)->next = NULL;
}

void	ft_error_export(int option, char *str)
{
	if ((ft_strlen(str) >= 2) && str[0] == '-' && option != -1)
	{
		display_error(ERR_EXPORT_INV, str);
		g_exit_status = 1;
	}
	else if (option != -1)
	{
		display_error(ERR_EXPORT_ARG, str);
		g_exit_status = 1;
	}
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
