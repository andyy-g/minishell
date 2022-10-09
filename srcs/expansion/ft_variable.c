/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_variable.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 15:48:08 by charoua           #+#    #+#             */
/*   Updated: 2022/10/09 07:16:04 by agranger         ###   ########.fr       */
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
	char	*new;

	new = ft_fill_new(str, var, start, end);
	free(str);
	return (new);
}

int	ft_check_variable(t_pars **exp, int i, int j, int *error)
{
	char	**str;
	t_env	*env;

	env = singleton_env(1, NULL, NULL);
	str = &((*exp)->str);
	while (i - 1 > j && env)
	{
		if (env->var && ft_ncmp((*str) + j + 1, env->var, i - j - 1))
		{
			if (!check_ambiguous_redirect(exp, env, *str, error))
				*str = ft_replacebyvar(*str, env->value, j, i);
			return (1);
		}
		env = env->next;
	}
	if (i == j + 2 && (*str)[i - 1] && (*str)[i - 1] == '?')
		return (expand_exit_status(str, i, j));
	return (0);
}

void	complete_str(t_pars **exp, int *j, int i, int *error)
{
	int	save;

	if (!*j && !(*exp)->str[i])
	{
		if ((*exp)->prev && (is_chevron((*exp)->prev->token)
				&& (*exp)->prev->token != HEREDOC))
		{
			*error = 1;
			display_error(ERR_AMB_REDIRECT, (*exp)->str);
		}
		(*exp)->token = NONE;
		*j = i - 1;
		return ;
	}
	save = *j;
	while (*j < i && (*exp)->str[i])
		(*exp)->str[(*j)++] = (*exp)->str[i++];
	(*exp)->str[*j] = '\0';
	*j = save - 1;
}

int	ft_variable(t_pars **exp, int *j, int *error, bool dquote)
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
	found = ft_check_variable(exp, i, *j, error);
	if (!(*exp)->str || found == -1)
		return (0);
	if (found == 2)
	{
		*error = 1;
		return (1);
	}
	if (found == 0)
		complete_str(exp, j, i, error);
	else
		if (!word_splitting(exp, dquote))
			return (0);
	return (1);
}
