/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 16:28:46 by agranger          #+#    #+#             */
/*   Updated: 2022/10/06 12:17:55 by agranger         ###   ########.fr       */
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

void	push_back_redir(t_pars *token, t_pars *last)
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
}

void	go_to_rpar(t_pars **token)
{
	int	bracket;

	bracket = 1;
	*token = (*token)->next;
	while ((*token)->str && bracket)
	{
		if ((*token)->token == RPAR)
			bracket--;
		else if ((*token)->token == LPAR)
			bracket++;
		*token = (*token)->next;
	}
}

bool	redir_in_order(t_pars **token)
{
	if ((*token)->token == LPAR)
		go_to_rpar(token);
	else if ((*token)->token == WORD)
	{
		while ((*token)->str && (*token)->token == WORD)
			*token = (*token)->next;
	}
	while ((*token)->str && is_redir_token(*token))
		*token = (*token)->next;
	if (!(*token)->str || (*token)->token != WORD)
		return (true);
	return (false);
}

t_pars	*put_redirs_in_order(t_pars *token)
{
	t_pars	*first;

	while (token->str)
	{
		first = token;
		if (redir_in_order(&token))
		{
			if (token->str)
				token = token->next;
			continue ;
		}
		push_back_redir(first, token);
	}
	while (token->prev)
		token = token->prev;
	return (token);
}
