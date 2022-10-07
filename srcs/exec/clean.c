/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 11:00:59 by agranger          #+#    #+#             */
/*   Updated: 2022/10/07 21:23:52 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fd_in_out(t_node *node)
{
	if (node->fd_in != STDIN_FILENO)
		close(node->fd_in);
	if (node->fd_out != STDOUT_FILENO)
		close(node->fd_out);
}

void	clean_heredoc(t_pars *token, t_node *node)
{
	if (token && token->heredoc)
	{
		close(token->heredoc[READ]);
		ft_free(token->heredoc);
		token->heredoc = NULL;
	}
	if (node && node->heredoc)
	{
		close(node->heredoc[READ]);
		ft_free(node->heredoc);
		node->heredoc = NULL;
	}
}

void	close_fds_exec_fail(t_node *node)
{
	if (node->parent)
		clean_heredoc(NULL, node->parent);
	if (node->fd_in != STDIN_FILENO)
	{
		close(node->fd_in);
		close(STDIN_FILENO);
	}
	if (node->fd_out != STDOUT_FILENO)
	{
		close(node->fd_out);
		close(STDOUT_FILENO);
	}
}

void	free_tokens_ast(t_node **ast, t_dblist **tokens)
{
	if (*tokens)
	{
		ft_free_tokens(*tokens);
		*tokens = NULL;
	}
	if (*ast)
	{
		ast_delete_nodes(*ast);
		*ast = NULL;
	}
}
