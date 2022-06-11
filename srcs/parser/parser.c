/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 10:13:02 by agranger          #+#    #+#             */
/*   Updated: 2022/06/11 18:32:07 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ls || cat Makefile | grep srcs && pwd | wc

/* FCT AST

   t_node	*ast_create_node(t_pars **token);
   void		ast_add_children(t_node *parent, t_node *left_child, t_node *rigth_child);
   void		ast_delete_nodes(t_node *node);
   void		ast_add_arg_cmd(t_node **first, t_node *new);

 */

t_node	*cmd_line(t_pars **token)
{
	t_node	*ret;
	t_pars	*save;

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

t_node	*cmd_line1(t_pars **token)
{
	t_node	*left;
	t_node	*right;
	t_node	*logical_op;
	t_pars	*save;

	save = *token;
	left = cmd_pipe(token);
	if (!left)
	{
		*token = save;
		return (NULL);
	}
	if ((*token)->token != AND && (*token)->token != OR)
	{
		ast_delete_nodes(left);
		*token = save;
		return (NULL);
	}
	logical_op = ast_create_node(token);
	*token = (*token)->next;
	right = cmd_line(token);
	if (!right)
	{
		ast_delete_nodes(left);
		ast_delete_nodes(logical_op);
		*token = save;
		return (NULL);
	}
	ast_add_children(logical_op, left, right);
	return (logical_op);
}

t_node	*cmd_line2(t_pars **token)
{
	return (cmd_pipe(token));
}

t_node	*cmd_pipe(t_pars **token)
{
	t_node	*ret;
	t_pars	*save;

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

t_node	*cmd_pipe1(t_pars **token)
{
	t_node	*left;
	t_node	*right;
	t_node	*pipe;
	t_pars	*save;

	save = *token;
	left = cmd_redir(token);
	if (!left)
	{
		*token = save;
		return (NULL);
	}
	if ((*token)->token != PIPE)
	{
		ast_delete_nodes(left);
		*token = save;
		return (NULL);
	}
	pipe = ast_create_node(token);
	*token = (*token)->next;
	right = cmd_pipe(token);
	if (!right)
	{
		ast_delete_nodes(left);
		ast_delete_nodes(pipe);
		*token = save;
		return (NULL);
	}
	ast_add_children(pipe, left, right);
	return (pipe);
}

t_node	*cmd_pipe2(t_pars **token)
{
	return (cmd_redir(token));
}

t_node	*cmd_redir(t_pars **token)
{
	t_node	*ret;
	t_pars	*save;

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

t_node	*cmd_redir1(t_pars **token)
{
	t_node	*redir;
	t_node	*right;
	t_node	*left;
	t_pars	*save;

	save = *token;
	left = cmd(token);
	if (!left)
	{
		*token = save;
		return (NULL);
	}
	if ((*token)->token > 1 && (*token)->token < 6)
	{
		ast_delete_nodes(left);
		*token = save;
		return (NULL);
	}
	redir = ast_create_node(token);
	*token = (*token)->next;
	right = file(token);
	if (!right)
	{
		ast_delete_nodes(left);
		ast_delete_nodes(redir);
		*token = save;
		return (NULL);
	}
	ast_add_children(redir, left, right);
	return (redir);
}

t_node	*cmd_redir2(t_pars **token)
{
	t_node	*redir;
	t_node	*right;
	t_node	*left;
	t_pars	*save;

	save = *token;
	if ((*token)->token > 1 && (*token)->token < 6)
	{
		*token = save;
		return (NULL);
	}
	redir = ast_create_node(token);
	*token = (*token)->next;
	right = file(token);
	if (!right)
	{
		ast_delete_nodes(redir);
		*token = save;
		return (NULL);
	}
	left = cmd(token);
	if (!left)
	{
		ast_delete_nodes(right);
		ast_delete_nodes(redir);
		*token = save;
		return (NULL);
	}
	ast_add_children(redir, left, right);
	return (redir);
}

t_node	*cmd_redir3(t_pars **token)
{
	return (cmd(token));
}

t_node	*cmd(t_pars **token)
{
	t_node	*ret;
	t_pars	*save;

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

t_node	*cmd1(t_pars **token)
{
	t_node	*ret;
	t_pars	*save;

	save = *token;
	if ((*token)->token != LPAR)
	{
		*token = save;
		return (NULL);
	}
	*token = (*token)->next;
	ret = cmd_line(token);
	if ((*token)->token != RPAR)
	{
		ast_delete_nodes(ret);
		*token = save;
		return (NULL);
	}
	return (ret);
}

t_node	*cmd2(t_pars **token)
{
	t_node	*cmd;
	t_node	*tmp;

	if (!*token)
		return (NULL);
	cmd = ast_create_node(token);
	*token = (*token)->next;
	while (*token && (*token)->token == WORD)
	{
		tmp = ast_create_node(token);
		ast_add_arg_cmd(&cmd, tmp);
		*token = (*token)->next;
	}
	return (cmd);
}

t_node	*file(t_pars **token)
{
	t_node	*file;

	if (!*token)
		return (NULL);
	file = ast_create_node(token);
	*token = (*token)->next;
	return (file);
}

int	parser(t_node **ast, t_pars *token)
{
	*ast = cmd_line(&token);
	if (!ast)
		return (1);
	return (0);
}
