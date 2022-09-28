/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 11:07:42 by agranger          #+#    #+#             */
/*   Updated: 2022/09/28 18:16:52 by agranger         ###   ########.fr       */
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

int	launch_heredoc(t_pars *token, int *pipe_heredoc, char *lim, t_sa *sig)
{
	char	*input;
	bool	expansion;
	int		i;
	int		ret;

	if (!set_signal(SIGINT, HDOC, sig[INT])
		|| !set_signal(SIGQUIT, HDOC, sig[QUIT]))
		return (0);
	expansion = must_be_expanded(lim);
	i = 0;
	while (1)
	{
		input = readline("> ");
		i++;
		ret = is_eof_heredoc(input, lim, i);
		if (ret == -1)
			return (0);
		if (ret)
		{
			clean_heredoc(token, NULL);
			break ;
		}
		if (!ft_strcmp(lim, input))
			break ;
		if (expansion)
		{
			input = heredoc_expansion(input);
			if (!input)
				return (0);
			write(pipe_heredoc[WRITE], input, ft_strlen(input));
			ft_free(input);
		}
		else
			write(pipe_heredoc[WRITE], input, ft_strlen(input));
		write(pipe_heredoc[WRITE], "\n", 1);
	}
	clean_heredoc(token, NULL);
	token->heredoc = pipe_heredoc;
	close(pipe_heredoc[WRITE]);
	return (1);
}

int	check_is_heredoc(t_pars *token, char *lim, t_sa sig[2])
{
	int	*pipe_heredoc;
	int	ret;

	ret = 1;
	if (!lim)
		return (0);
	if (token && token->str && token->token == HEREDOC)
	{
		pipe_heredoc = malloc(sizeof(int) * 2);
		if (pipe(pipe_heredoc) == -1)
		{
			perror("pipe");
			ft_free(pipe_heredoc);
			ft_free(lim);
			return (0);
		}
		if (!launch_heredoc(token, pipe_heredoc, lim, sig))
		{
			ft_free(pipe_heredoc);
			ret = 0;
		}
		if (!set_signal(SIGINT, EXEC, sig[INT])
			|| !set_signal(SIGQUIT, EXEC, sig[QUIT]))
			return (0);
	}
	ft_free(lim);
	return (ret);
}
