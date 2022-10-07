/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:19:50 by agranger          #+#    #+#             */
/*   Updated: 2022/10/07 19:32:01 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_next_cmd_logical_node(t_node **cmd, pid_t **pids, int *index_cmd)
{
	if (!check_logical_node(cmd, *pids))
	{
		*cmd = next_cmd(*cmd);
		(*index_cmd)++;
	}
	else
	{
		ft_free(*pids);
		*pids = init_pid_arr(*cmd, index_cmd);
	}
}

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
		if (!exec_cmd_fork(ast, pid))
			return (0);
	}
	else
	{
		if (ast->is_pipe)
			close(pipe_fd[WRITE]);
		close_fd_in_out(ast);
		(*pids)[index_cmd] = pid;
	}
	return (1);
}

int	tree_traversal(t_node *cmd, int *pipe_fd, pid_t **pids, int index_cmd)
{
	int	ret;

	while (cmd)
	{
		ret = init_fd(cmd, pipe_fd);
		if (ret == 2)
		{
			if (cmd->is_pipe)
				close(pipe_fd[WRITE]);
			close_fd_in_out(cmd);
			cmd = next_cmd_after_redir(cmd);
			continue ;
		}
		if (!ret || !fork_process(cmd, pipe_fd, pids, index_cmd))
			return (0);
		clean_heredoc(NULL, cmd->parent);
		is_next_cmd_logical_node(&cmd, pids, &index_cmd);
	}
	close(pipe_fd[READ]);
	return (1);
}

int	launch_exec_fork(t_node *cmd, t_sa *sig)
{
	int		status;
	int		index_cmd;
	int		pipe_fd[2];
	pid_t	*pids;

	pipe_fd[READ] = -1;
	pipe_fd[WRITE] = -1;
	pids = init_pid_arr(cmd, &index_cmd);
	if (!pids)
		return (0);
	if (!set_signals_exec(cmd->cmd[0], sig))
		return (0);
	if (!tree_traversal(cmd, pipe_fd, &pids, index_cmd))
		return (0);
	status = get_status_last_process(pids);
	g_exit_status = status;
	ft_free(pids);
	return (1);
}

int	exec(t_node *ast, t_sa *sig)
{
	int	ret;

	move_to_first_cmd(&ast);
	if (is_builtin_no_fork(ast->cmd[0]) && is_uniq_cmd(ast))
	{	
		ret = init_fd(ast, NULL);
		if (ret == 2)
			return (1);
		if (!ret || !exec_cmd_wo_fork(ast))
			return (0);
		return (1);
	}
	if (!launch_exec_fork(ast, sig))
		return (0);
	return (1);
}
