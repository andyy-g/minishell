/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:19:50 by agranger          #+#    #+#             */
/*   Updated: 2022/07/19 01:54:35 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/********* TOOLS ********/

bool	is_builtin(char *cmd)
{
	(void)cmd;
	return (false);
}

bool	is_uniq_cmd(t_node *node)
{
	(void)node;
	return (false);
}

/************************/

int	look_for_heredocs(t_node *ast)
{
	(void)ast;
	return (1);
}

int	exec_builtin(t_node *ast)
{
	(void)ast;
	return (1);
}

int	exec_bin(t_node *ast)
{
	(void)ast;
	return (1);
}

t_node	*next_cmd(t_node *ast)
{
	return (ast);
}

int	child_process(t_node *ast)
{
	//REDIR ??????????
	if (is_builtin(ast->cmd[0]))
	{
		if (!exec_builtin(ast))
			return (0);
		return (1);
	}
	else
	{
		exec_bin(ast);
	}
	return (1);
}

int	parent_process(t_node *ast)
{
	(void)ast;	
	return (1);
}

int	exec_w_fork(t_node *ast)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (0);
	}
	if (pid == 0)
	{
		if (!child_process(ast))
			return (0);
	}
	else
	{
		if (!parent_process(ast)) //fct nécessaire ? que fait le parent process ?
			return (0);
	}
	return (1);
}

int	exec(t_node *ast)
{
	int ret;
	int	cmd_run;	//compteur de cmd run

	if (!look_for_heredocs(ast)) 	//chercher heredocs dans tout l'arbre (même derrière || et &&) et les lancer
		return (0);					//lancer look_for_heredocs à l'exit des syntax errors (<< lim cat < >)
	if (is_builtin(ast->cmd[0]) && is_uniq_cmd(ast)) 	//conditions supplémentaires ? 
	{													//(export ? redir out ? ...) + sortir de la loop
		if (!exec_builtin(ast))
			return (0);
		return (1);
	}
	ret = 0;
	cmd_run = 0;
	while (ast)
	{
		ret = exec_w_fork(ast);
		if (!ret)
			return (0);
		if (ret == 1)
			cmd_run++;
		ast = next_cmd(ast);
	}
	//wait nb cmd run -> wait/wait3/wait4/waitpid
	return (1);
}
