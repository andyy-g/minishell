/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:19:50 by agranger          #+#    #+#             */
/*   Updated: 2022/10/09 04:29:44 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fork_process(t_node *ast, int *pipe_fd, pid_t **pids, int index_cmd)
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
		free(*pids);
		if (ast->is_pipe)
			close(pipe_fd[READ]);
		exec_cmd_fork(ast, pid);
		return (0);
	}
	else
	{
		if (ast->is_pipe)
			close(pipe_fd[WRITE]);
		close_fd_in_out(ast);
		(*pids)[index_cmd] = pid;
		return (1);
	}
}

int	tree_traversal(t_node **cmd, int *pipe_fd, pid_t **pids, int *index_cmd)
{
	int	ret;

	while (*cmd && (*cmd)->type != AND && (*cmd)->type != OR)
	{
		ret = init_fd(*cmd, pipe_fd);
		if ((*cmd)->type == NONE || ret == 2)
		{
			skip_and_clean(cmd, index_cmd, pipe_fd, ret);
			continue ;
		}
		if (!ret || !fork_process(*cmd, pipe_fd, pids, *index_cmd))
			return (0);
		free_heredoc(NULL, *cmd);
		*cmd = next_cmd(*cmd);
		(*index_cmd)++;
	}
	close(pipe_fd[READ]);
	return (1);
}

int	launch_exec_fork(t_node **cmd, t_sa *sig, pid_t **pids, int *index_cmd)
{
	int		status;
	int		pipe_fd[2];

	pipe_fd[READ] = -1;
	pipe_fd[WRITE] = -1;
	if (!set_signals_exec((*cmd)->cmd[0], sig))
		return (0);
	if (!tree_traversal(cmd, pipe_fd, pids, index_cmd))
		return (0);
	status = get_status_last_process(*pids);
	g_exit_status = status;
	return (1);
}

int	launch_exec_wo_fork(t_node *ast)
{
	int	ret;

	ret = init_fd(ast, NULL);
	if (ret != 2)
	{
		if (!ret || !exec_cmd_wo_fork(ast))
			return (0);
	}
	else
		g_exit_status = 1;
	return (1);
}

int	exec(t_node *ast, t_sa *sig)
{
	pid_t	*pids;
	int		index_cmd;

	move_to_first_cmd(&ast);
	pids = init_pid_arr(ast, &index_cmd);
	if (!pids)
		return (0);
	while (ast)
	{
		if (ast->type == NONE)
			ast = next_cmd(ast);
		else if (is_builtin_no_fork(ast->cmd[0]) && is_uniq_cmd(ast))
		{	
			ft_free(pids);
			pids = NULL;
			if (!launch_exec_wo_fork(ast))
				return (0);
			ast = next_cmd(ast);
		}
		else if (!launch_exec_fork(&ast, sig, &pids, &index_cmd))
			return (0);
		check_logical_node(&ast, &pids, &index_cmd);
	}
	ft_free(pids);
	return (1);
}
