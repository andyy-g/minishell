/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 12:57:20 by charoua           #+#    #+#             */
/*   Updated: 2022/09/22 14:26:20 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	trim_dollar_quotes(char *str, char quote)
{
	int	i;
	int	j;
	int	match;

	i = 0;
	j = 2;
	match = 0;
	while (str[j])
	{
		while (str[j] && str[j] == quote && !match)
		{
			j++;
			match = 1;
		}
		if (!str[j])
			break ;
		str[i] = str[j];
		i++;
		j++;
	}
	str[i] = '\0';
}

bool	is_env_variable(t_pars *exp, int i)
{
	if (i == -1)
		return (true);
	if (exp->str[i] == '$' && exp->str[i + 1]
		&& (ft_isalnum(exp->str[i + 1]) || exp->str[i + 1] == '_'
			|| exp->str[i + 1] == '?'))
		return (true);
	return (false);
}

bool	must_be_trim(t_pars *exp, int i)
{
	if (exp->str[i] == '$' && exp->str[i + 1]
		&& (exp->str[i + 1] == '\'' || exp->str[i + 1] == '"'))
		return (true);
	return (false);
}

int	check_env_var(t_pars **exp, t_env **env, int *i)
{
	while ((*exp)->str[*i] == 39 || (*exp)->str[*i] == 34)
		*i = ft_exp_quote(exp, &(*env), *i, (*exp)->str[*i]);
	if (is_env_variable(*exp, *i) && *env)
	{
		if (!ft_variable(exp, *env, i))
			return (0);
	}
	else if (must_be_trim(*exp, *i))
		trim_dollar_quotes(&(*exp)->str[*i], (*exp)->str[*i + 1]);
	return (1);
}

int	ft_expand(t_dblist **list, t_env **env)
{
	t_pars	*exp;
	int		i;

	exp = (*list)->first;
	while (exp && exp->next)
	{
		i = 0;
		if (exp->str[i] == '~' && !(exp->str[i + 1]) && *env)
			ft_home(&exp, *env);
		while (exp->str && exp->str[i] != '\0')
		{
			if (!check_env_var(&exp, env, &i))
				return (0);
			i++;
		}
		if (exp->str && !ft_check_wildcard(&exp, list))
			return (0);
		if (exp->next)
			exp = exp->next;
	}
	while (exp && exp->prev)
		exp = exp->prev;
	(*list)->first = exp;
	return (1);
}
