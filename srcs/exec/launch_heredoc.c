/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 11:07:42 by agranger          #+#    #+#             */
/*   Updated: 2022/10/07 19:18:38 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_var_heredoc(char *var)
{
	t_env	*env;

	env = singleton_env(1, NULL, NULL);
	while (env && ft_strcmp(env->var, var))
		env = env->next;
	if (!env)
		return (NULL);
	return (env->value);
}

char	*replace_exp_heredoc(char *str, int start, int len, char *var)
{
	char	*ret;
	int		size;
	int		i;

	size = ft_strlen(str) - len + ft_strlen(var);
	ret = malloc(sizeof(char) * (size + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (str[i] && i < start)
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	ft_strcat(ret, var);
	ft_strcat(ret, &str[i + len]);
	return (ret);
}

char	*heredoc_expansion(char *str)
{
	int		pos;
	int		size;
	char	*ret;
	char	*var;
	char	*value;

	pos = 0;
	while (str[pos] && str[pos] != '$')
		pos++;
	if (!str[pos])
		return (ft_strdup(str));
	size = 0;
	pos++;
	while (str[pos + size] && (ft_isalnum(str[pos + size])
			|| str[pos + size] == '_'))
		size++;
	var = ft_substr(str, pos, size);
	if (!var)
		return (NULL);
	value = get_env_var_heredoc(var);
	if (!value)
		return (ft_strdup(str));
	ret = replace_exp_heredoc(str, pos - 1, size + 1, value);
	return (ret);
}

int	launch_heredoc(t_dblist **list, int *pipe_heredoc, char *lim, t_sa *sig)
{
	pid_t	pid;
	t_pars	*token;

	token = (*list)->curr;
	if (!set_signal(IGN, sig))
		return (0);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (0);
	}
	if (pid == 0)
	{
		if (!fork_heredoc_child(list, sig, pipe_heredoc, lim))
			return (0);
	}
	if (!fork_heredoc_parent(pid, token, pipe_heredoc))
		return (-2);
	return (1);
}

int	check_is_heredoc(char *lim, t_sa *sig, t_dblist **list)
{
	int		ret;
	t_pars	*token;

	token = (*list)->curr;
	ret = 1;
	if (!lim)
		return (-1);
	if (token && token->str && token->token == HEREDOC)
		ret = pipe_heredoc(lim, list, sig);
	ft_free(lim);
	return (ret);
}
