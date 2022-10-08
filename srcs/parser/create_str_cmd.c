/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_str_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 23:22:59 by agranger          #+#    #+#             */
/*   Updated: 2022/10/08 23:35:34 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_size_cmd(t_pars *cmd, t_toktype *type, int *tok)
{
	int		i;

	if (*type != WORD && *type != NONE)
		return (1);
	i = 0;
	*tok = 0;
	while (cmd->str && (cmd->token == WORD || cmd->token == NONE))
	{
		if (cmd->token == WORD)
			i++;
		cmd = cmd->next;
		(*tok)++;
	}
	if (i && *type == NONE)
		*type = WORD;
	return (i);
}

char	**unique_str_cmd(t_pars **cmd, int *status, char **ret)
{
	int	i;

	i = 0;
	ret[i] = ft_strdup((*cmd)->str);
	if (!ret[i])
		return (set_status_error(status, ret));
	i++;
	*cmd = (*cmd)->next;
	ret[i] = NULL;
	return (ret);
}

char	**multiple_str_cmd(t_pars **cmd, int *status, char **ret)
{
	int	i;

	i = 0;
	while ((*cmd)->str
		&& ((*cmd)->token == NONE || (*cmd)->token == WORD))
	{
		if ((*cmd)->token == WORD)
		{
			ret[i] = ft_strdup((*cmd)->str);
			if (!ret[i])
				return (set_status_error(status, ret));
			i++;
		}
		*cmd = (*cmd)->next;
	}
	ret[i] = NULL;
	return (ret);
}

char	**create_simple_cmd(t_pars **cmd, int *status, t_toktype *type)
{
	char	**ret;
	int		size;
	int		tok;

	*status = 1;
	if (!cmd)
		return (NULL);
	size = get_size_cmd(*cmd, type, &tok);
	if (!size)
	{
		while (tok--)
			*cmd = (*cmd)->next;
		return (NULL);
	}
	ret = malloc(sizeof(char *) * (size + 1));
	if (!ret)
		return (set_status_error(status, NULL));
	if (size == 1)
		return (unique_str_cmd(cmd, status, ret));
	else
		return (multiple_str_cmd(cmd, status, ret));
}
