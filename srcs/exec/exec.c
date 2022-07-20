/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:19:50 by agranger          #+#    #+#             */
/*   Updated: 2022/07/20 16:06:19 by agranger         ###   ########.fr       */
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

t_node	*next_cmd(t_node *ast)
{
	return (ast);
}

bool	is_first_cmd(t_node *node)
{
	(void)node;
	return (false);
}

bool	is_last_cmd(t_node *node)
{
	(void)node;
	return (false);
}

/************************/

bool	check_logical_node(t_node *node)
{
	//if redir -> up
	//if !parent
	//if pipe -> true
	//if and || or -> fct wait + check exit status :
	//if and 
	//	if g_exit_status == 0 -> true
	//else
	//	-> false
	//if or
	//	if g_exit_status == 0 -> false
	(void)node;
	return (false);
}

int	look_for_heredocs(t_node *ast)
{
	(void)ast;
	return (1);
}

int	exec_builtin(t_node *ast, int (*fct)(t_node *node))
{
	(void)fct;
	(void)ast;
	return (1);
}

int	exec_bin(t_node *ast)
{
	(void)ast;
	return (1);
}

int	init_fd(t_node *node)
{
	int	pipe_fd[2];

	//existence file_in
	//	if ! return (2);
	//créer les file_out
	//init fd_in et fd_out
	//fct utiles : is_first_cmd / is_last_cmd
	//enum READ WRITE
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	(void)node;
	(void)pipe_fd;
	return (1);
}

bool	cmd_is(char *cmd, char *builtin)
{
	if (!ft_strcmp(cmd, builtin))
		return (true);
	return (false);
}

int	exec_cmd(t_node *node)
{
	int	ret;

	if (cmd_is(node->cmd[0], "echo"))
		ret = exec_builtin(node, ft_echo);
	else if (cmd_is(node->cmd[0], "pwd"))
		ret = exec_builtin(node, ft_pwd);
	else if (cmd_is(node->cmd[0], "cd"))
		ret = exec_builtin(node, ft_cd);
	else if (cmd_is(node->cmd[0], "export"))
		ret = exec_builtin(node, ft_export);
	else if (cmd_is(node->cmd[0], "unset"))
		ret = exec_builtin(node, ft_unset);
	else if (cmd_is(node->cmd[0], "env"))
		ret = exec_builtin(node, ft_env);
	else if (cmd_is(node->cmd[0], "exit"))
		ret = exec_builtin(node, ft_exit);
	else
		ret = exec_bin(node);
	return (ret);
}

int	fork_process(t_node *ast)
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
		if (!exec_cmd(ast))
			//execve fail, do something
		else
			//builtin, exit child_process
	}
	else
	{
		//close fds
	}
	return (1);
}

int	tree_traversal(t_node *ast)
{
	int	ret;

	while (ast)
	{
		ret = init_fd(ast);
		if (ret == 2)
		{
			ast = next_cmd(ast);
			continue ;
		}
		if (!ret || !fork_process(ast))
			return (0);
		if (!check_logical_node(ast->parent))
			break ;
		ast = next_cmd(ast);
	}
}

int	exec(t_node *ast)
{
	if (!look_for_heredocs(ast)) 	//chercher heredocs dans tout l'arbre (même derrière || et &&) et les lancer
		return (0);					//lancer look_for_heredocs à l'exit des syntax errors (<< lim cat < >)
	if (is_builtin(ast->cmd[0]) && is_uniq_cmd(ast)) 	//conditions supplémentaires ? 
	{													//(export ? redir out ? ...)
		if (!exec_cmd(ast))
			return (0);
		return (1);
	}
	if (!tree_traversal(ast))
		return (0);
	//wait_exec -> while (wait(&status) > 0);
	return (1);
}
