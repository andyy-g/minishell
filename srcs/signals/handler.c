/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 18:42:46 by agranger          #+#    #+#             */
/*   Updated: 2022/09/29 14:40:39 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_input(int signum)
{
	write(1, "\n", 1); 
	rl_on_new_line();
	rl_replace_line("", 0); 
	rl_redisplay();
	g_exit_status = 128 + signum;
	return ;
}

void	sigint_exec(int signum)
{
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	g_exit_status = 128 + signum;
	return ;
}

void	sigint_hdoc(int signum)
{
	g_exit_status = 128 + signum;
	return ;
}

void	sigpipe_hdoc(int signum)
{
	(void)signum;
	return ;
}

void	sigquit_exec(int signum)
{
	write(2, "Quit (core dumped)\n", ft_strlen("Quit (core dumped)\n"));
	g_exit_status = 128 + signum;
	return ;
}