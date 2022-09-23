/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_wo_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 12:11:42 by agranger          #+#    #+#             */
/*   Updated: 2022/09/23 12:29:07 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	cmd_is(char *cmd, char *builtin)
{
	if (!ft_strcmp(cmd, builtin))
		return (true);
	return (false);
}

bool	is_builtin_no_fork(char *cmd)
{
	if (cmd_is(cmd, "cd")
		|| cmd_is(cmd, "unset")
		|| cmd_is(cmd, "exit"))
		return (true);
	if (cmd_is(cmd, "export") && !cmd[1])
		return (true);
	if (cmd_is(cmd, "<")
		|| cmd_is(cmd, "<<")
		|| cmd_is(cmd, ">")
		|| cmd_is(cmd, ">>"))
		return (true);
	return (false);
}

bool	is_uniq_cmd(t_node *node)
{
	while (node->parent)
		node = node->parent;
	if (is_chevron(node->type) || node->type == WORD)
		return (true);
	return (false);
}

int	exec_builtin(t_node *ast, int (*ft_builtin)(t_node *node))
{
	int	ret;

	ret = ft_builtin(ast);
	return (ret);
}

int	exec_cmd_wo_fork(t_node *node)
{
	int	ret;

	ret = 1;
	if (cmd_is(node->cmd[0], "cd"))
		printf("CD\n");
	else if (cmd_is(node->cmd[0], "export"))
		printf("EXPORT\n");
	else if (cmd_is(node->cmd[0], "unset"))
		printf("UNSET\n");
	else if (cmd_is(node->cmd[0], "exit"))
		printf("EXIT\n");
	close_fd_in_out(node);
	clean_heredoc(NULL, node);
	return (ret);
}
