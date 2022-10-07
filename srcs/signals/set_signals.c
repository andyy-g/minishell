/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_signals.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 15:27:48 by agranger          #+#    #+#             */
/*   Updated: 2022/10/07 20:31:18 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	convert_status(int status)
{
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = WTERMSIG(status) + 128;
	else if (WIFSTOPPED(status))
		status = WSTOPSIG(status) + 128;
	return (status);
}

int	set_signal_sigint(t_context context, t_sa sa)
{
	ft_memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (context == INPUT)
		sa.sa_handler = &sigint_input;
	if (context == EXEC)
		sa.sa_handler = &sigint_exec;
	if (context == HDOC)
		sa.sa_handler = &sigint_hdoc;
	if (context == IGN)
		sa.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (0);
	}
	return (1);
}

int	set_signal_sigquit(t_context context, t_sa sa)
{
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (context == INPUT || context == HDOC || context == IGN)
		sa.sa_handler = SIG_IGN;
	if (context == EXEC)
		sa.sa_handler = &sigquit_exec;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (0);
	}
	return (1);
}

int	set_signal_sigpipe(t_context context, t_sa sa)
{
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (context == INPUT || context == EXEC || context == IGN)
		sa.sa_handler = SIG_DFL;
	if (context == HDOC)
		sa.sa_handler = &sigpipe_hdoc;
	if (sigaction(SIGPIPE, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (0);
	}
	return (1);
}

int	set_signal(t_context context, t_sa *sig)
{
	if (!set_signal_sigint(context, sig[INT]))
		return (0);
	if (!set_signal_sigquit(context, sig[QUIT]))
		return (0);
	if (!set_signal_sigpipe(context, sig[SPIPE]))
		return (0);
	return (1);
}
