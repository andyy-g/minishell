/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 11:08:05 by agranger          #+#    #+#             */
/*   Updated: 2022/10/09 03:40:39 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mark_chars_to_trim(char *lim)
{
	int		i;
	char	quote;

	i = 0;
	while (lim[i])
	{
		if (lim[i] == '\'' || lim[i] == '"')
		{
			quote = lim[i];
			lim[i] = -lim[i];
			if (i > 0 && lim[i - 1] == '$' && (i == 1 || lim[i - 2] != '$'))
				lim[i - 1] = -lim[i - 1];
			while (lim[i] && lim[i] != quote)
				i++;
			lim[i] = -lim[i];
		}
		if (lim[i])
			i++;
	}
}

void	trim_lim(char *lim)
{
	int	i;
	int	skip;

	mark_chars_to_trim(lim);
	i = 0;
	skip = 0;
	while (lim[skip])
	{
		while (lim[skip] && lim[skip] < 0)
			skip++;
		if (!lim[skip])
			break ;
		lim[i] = lim[skip];
		i++;
		skip++;
	}
	lim[i] = '\0';
}

bool	must_be_expanded(char *lim)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (lim[i])
	{
		if (lim[i] == '\'' || lim[i] == '"')
		{
			quote = lim[i];
			trim_lim(lim);
			break ;
		}
		i++;
	}
	if (quote)
		return (false);
	return (true);
}

void	free_heredoc(t_pars *tok, t_node *node)
{
	while (tok)
	{
		clean_heredoc(tok, NULL);
		tok = tok->prev;
	}
	while (node)
	{
		clean_heredoc(NULL, node);
		node = node->parent;
	}
	return ;
}
