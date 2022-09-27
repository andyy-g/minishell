/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 15:27:48 by agranger          #+#    #+#             */
/*   Updated: 2022/09/26 17:48:40 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1); 
		rl_on_new_line();
		rl_replace_line("", 0); 
		rl_redisplay();
	}
}


int	init_signals(void)
{
	struct sigaction	sigint;
	struct sigaction	sigquit;

	ft_memset(&sigint, 0, sizeof(sigint));
	ft_memset(&sigquit, 0, sizeof(sigquit));
	sigint.sa_handler = &handle_sigint;
	sigint.sa_flags = SA_RESTART;
	sigemptyset(&sigint.sa_mask);
	if (sigaction(SIGINT, &sigint, NULL) == -1)
	{
		perror("sigaction");
		return (0);
	}
	sigquit.sa_handler = SIG_IGN;
	sigquit.sa_flags = SA_RESTART;
	sigemptyset(&sigquit.sa_mask);
	if (sigaction(SIGQUIT, &sigquit, NULL) == -1)
	{
		perror("sigaction");
		return (0);
	}
	return (1);
}
