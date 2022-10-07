/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 22:03:47 by charoua           #+#    #+#             */
/*   Updated: 2022/10/07 23:17:15 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			break ;
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
			break ;
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
		if (tmp)
			printf("declare -x %s", tmp->var);
		if (tmp && tmp->value)
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
