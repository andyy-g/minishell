/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 10:13:02 by agranger          #+#    #+#             */
/*   Updated: 2022/06/10 17:49:40 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ls || cat Makefile | grep srcs && pwd | wc

/* FCT AST

   t_node	*ast_create_node(t_token **token);
   void		ast_add_children(t_node *parent, t_node *left_child, t_node *rigth_child);
   void		ast_delete_nodes(t_node *node);
   void		ast_add_arg_cmd(t_node **first, t_node *new);

 */

t_node	*cmd_line(t_token **token)
{
	t_node	*ret;
	t_token	*save;

	save = *token;
	ret = cmd_line1(token);
	if (ret)
		return (ret);
	*token = save;
	ret = cmd_line2(token);
	if (ret)
		return (ret);
	*token = save;
	return (NULL);
}

t_node	*cmd_line1(t_token **token)
{
	t_node	*cmd_pipe;
	t_node	*cmd_line;
	t_node	*logical_op;
	t_token	*save;

	save = *token;
	cmd_pipe = cmd_pipe(token);
	if (!cmd_pipe)
	{
		*token = save;
		return (NULL);
	}
	if ((*token)->type != AND && (*token)->type != OR)
	{
		ast_delete_nodes(cmd_pipe);
		*token = save;
		return (NULL);
	}
	logical_op = ast_create_node(token);
	*token = (*token)->next;
	cmd_line = cmd_line(token);
	if (!cmd_line)
	{
		ast_delete_nodes(cmd_pipe);
		ast_delete_nodes(logical_op);
		*token = save;
		return (NULL);
	}
	ast_add_children(logical_op, cmd_pipe, cmd_line);
	return (logical_op);
}

t_node	*cmd_line2(t_token **token)
{
	return (cmd_pipe(token));
}

t_node	*cmd_pipe(t_token **token);
{
	t_node	*ret;
	t_token	*save;

	save = *token;
	ret = cmd_pipe1(token);
	if (ret)
		return (ret);
	*token = save;
	ret = cmd_pipe2(token);
	if (ret)
		return (ret);
	*token = save;
	return (NULL);
}

t_node	*cmd_pipe1(t_token **token)
{
	t_node	*cmd_redir;
	t_node	*cmd_pipe;
	t_node	*pipe;
	t_token	*save;

	save = *token;
	cmd_redir = cmd_redir(token);
	if (!cmd_redir)
	{
		*token = save;
		return (NULL);
	}
	if ((*token)->type != PIPE)
	{
		ast_delete_nodes(cmd_redir);
		*token = save;
		return (NULL);
	}
	pipe = ast_create_node(token);
	*token = (*token)->next;
	cmd_pipe = cmd_pipe(token);
	if (!cmd_pipe)
	{
		ast_delete_nodes(cmd_redir);
		ast_delete_nodes(pipe);
		*token = save;
		return (NULL);
	}
	ast_add_children(pipe, cmd_redir, cmd_pipe);
	return (pipe);
}

t_node	*cmd_pipe2(t_token **token)
{
	return (cmd_redir(token));
}

t_node	*cmd_redir(t_token **token)
{
	t_node	*ret;
	t_token	*save;

	save = *token;
	ret = cmd_redir1(token);
	if (ret)
		return (ret);
	*token = save;
	ret = cmd_redir2(token);
	if (ret)
		return (ret);
	*token = save;
	ret = cmd_redir3(token);
	if (ret)
		return (ret);
	*token = save;
	return (NULL);
}

t_node	*cmd_redir1(t_token **token)
{
	t_node	*cmd;
	t_node	*file;
	t_node	*redir;
	t_token	*token;

	save = *token;
	cmd = cmd(token);
	if (!cmd)
	{
		*token = save;
		return (NULL);
	}
	if ((*token)->type > 1 && (*token)->type < 6)
	{
		ast_delete_nodes(cmd);
		*token = save;
		return (NULL);
	}
	redir = ast_create_node(token);
	*token = (*token)->next;
	file = file(token);
	if (!file)
	{
		ast_delete_nodes(cmd);
		ast_delete_nodes(redir);
		*token = save;
		return (NULL);
	}
	ast_add_children(redir, cmd, file);
	return (redir);
}

t_node	*cmd_redir2(t_token **token)
{
	t_node	*cmd;
	t_node	*file;
	t_node	*redir;
	t_token	*token;

	save = *token;
	if ((*token)->type > 1 && (*token)->type < 6)
	{
		*token = save;
		return (NULL);
	}
	redir = ast_create_node(token);
	*token = (*token)->next;
	file = file(token);
	if (!file)
	{
		ast_delete_nodes(redir);
		*token = save;
		return (NULL);
	}
	cmd = cmd(token);
	if (!cmd)
	{
		ast_delete_nodes(cmd);
		ast_delete_nodes(redir);
		*token = save;
		return (NULL);
	}
	ast_add_children(redir, cmd, file);
	return (redir);
}

t_node	*cmd_redir3(t_token **token)
{
	return (cmd(token));
}

t_node	*cmd(t_token **token)
{
	t_node	*ret;
	t_token	*save;

	save = *token;
	ret = cmd1(token);
	if (ret)
		return (ret);
	*token = save;
	ret = cmd2(token);
	if (ret)
		return (ret);
	*token = save;
	return (NULL);
}

t_node	*cmd1(t_token **token)
{
	t_node	*cmd_line;
	t_token	*save;

	save = *token;
	if ((*token)->type != LPAR)
	{
		*token = save;
		return (NULL);
	}
	*token = (*token)->next;
	cmd_line = cmd_line(token);
	if ((*token)->type != RPAR)
	{
		ast_delete_nodes(cmd_line);
		*token = save;
		return (NULL);
	}
	return (cmd_line);
}

t_node	*cmd2(t_token **token)
{
	t_node	*cmd;
	t_node	*tmp;

	if (!*token)
		return (NULL);
	cmd = ast_create_node(token);
	*token = (*token)->next;
	while (*token && (*token)->type == WORD)
	{
		tmp = ast_create_node(token);
		ast_add_arg_cmd(&cmd, tmp);
		*token = (*token)->next;
	}
	return (cmd);
}

t_node	*file(t_token *token)
{
	t_node	*file;

	if (!*token)
		return (NULL);
	file = ast_create_node(token);
	*token = (*token)->next;
	return (file);
}

int	parser(t_node **ast, t_token *token)
{
	ast = cmd_line(&token);
	if (!ast)
		return (1);
	return (0);
}
