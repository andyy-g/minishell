/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_signals.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 15:27:48 by agranger          #+#    #+#             */
/*   Updated: 2022/09/28 16:22:13 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int set_signal_sigint(t_context context, t_sa sa)
{
	ft_memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (context == INPUT)
		sa.sa_handler = &sigint_input;
	if (context == EXEC || context == HDOC)
		sa.sa_handler = &sigint_exec;
	if (context == HDOC)
		sa.sa_handler = &sigint_hdoc;
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
	if (context == INPUT || context == HDOC)
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

int	set_signal(int sig, t_context context, t_sa sa)
{
	if (sig == SIGINT)
		if (!set_signal_sigint(context, sa))
			return (0);
	if (sig == SIGQUIT)
		if (!set_signal_sigquit(context, sa))
			return (0);
	return (1);
}
