/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 16:28:46 by agranger          #+#    #+#             */
/*   Updated: 2022/07/14 20:48:13 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap_redirs(t_pars *redir, t_pars *fd, t_pars **last)
{
	t_pars	*prev;
	t_pars	*next;
	t_pars	*end;

	prev = redir->prev;
	next = fd->next;
	if (prev)
		prev->next = next;
	next->prev = prev;
	end = (*last)->next;
	(*last)->next = redir;
	redir->prev = *last;
	fd->next = end;
	if (end->str)
		end->prev = fd;
	*last = fd;
}

t_pars	*push_back_redir(t_pars *token, t_pars *last)
{
	t_pars	*tmp;
	t_pars	*first;
	bool	is_first;

	first = NULL;
	is_first = true;
	while (token->str && (token->token == WORD || is_redir_token(token))
		&& token != first)
	{
		if (is_chevron(token->token))
		{
			tmp = token->next->next;
			if (token->next != last)
			{
				if (is_first)
					first = token;
				swap_redirs(token, token->next, &last);
				is_first = false;
			}
			token = tmp;
		}
		else
			token = token->next;
	}
	return (token);
}

t_pars	*put_redirs_in_order(t_pars *token)
{
	t_pars	*last;
	bool	contain_redir;

	last = token;
	contain_redir = false;
	while (last->next->str && (last->next->token == WORD
			|| is_redir_token(last->next)))
	{
		if (is_chevron(last->token))
			contain_redir = true;
		last = last->next;
	}
	if (contain_redir)
	{
		token = push_back_redir(token, last);
		while (token->prev)
			token = token->prev;
	}
	return (token);
}