/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 15:53:53 by agranger          #+#    #+#             */
/*   Updated: 2022/10/06 16:30:33 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_len_bytes(int bytes)
{
	if (bytes < WRITE_MAX)
		return (true);
	ft_putstr_fd("minishell: heredoc: heredoc too long\n", 2);
	return (false);
}

int	loop_heredoc(bool expansion, char *lim, int *pipe_heredoc, int bytes)
{
	char		*input;
	static int	i;

	while (1)
	{
		input = readline("> ");
		i++;
		bytes += ft_strlen(input);
		if (!check_len_bytes(bytes)
			|| is_eof_heredoc(input, lim, i) || !ft_strcmp(lim, input))
			break ;
		if (expansion)
		{
			input = heredoc_expansion(input);
			if (!input)
				return (0);
			write(pipe_heredoc[WRITE], input, ft_strlen(input));
			ft_free(input);
		}
		else
			write(pipe_heredoc[WRITE], input, ft_strlen(input));
		write(pipe_heredoc[WRITE], "\n", 1);
	}
	return (1);
}

int	fork_heredoc_child(t_dblist **list, t_sa *sig, int *pipe_heredoc, char *lim)
{
	bool		expansion;

	g_exit_status = 0;
	if (!set_signal(HDOC, sig))
		return (0);
	close(pipe_heredoc[READ]);
	expansion = must_be_expanded(lim);
	if (!loop_heredoc(expansion, lim, pipe_heredoc, 0))
		return (0);
	close(pipe_heredoc[WRITE]);
	ft_free(pipe_heredoc);
	ft_free(lim);
	free_tokens_ast(NULL, *list);
	exit_minishell(NULL);
	return (1);
}

int	fork_heredoc_parent(pid_t pid, t_pars *token, int *pipe_heredoc)
{
	int	status;

	waitpid(pid, &status, 0);
	status = convert_status(status);
	g_exit_status = status;
	clean_heredoc(token, NULL);
	token->heredoc = pipe_heredoc;
	close(pipe_heredoc[WRITE]);
	if (status == 130)
	{
		clean_heredoc(token, NULL);
		return (0);
	}
	return (1);
}
/*
   int launch_heredoc(t_dblist **list, int *pipe_heredoc, char *lim, t_sa *sig)
   {
   char            *input;
   bool            expansion;
   pid_t           pid;
   int           status)
   t_pars        *token;

   token = (*list)->curr;
   if (!set_signal(IGN, sig))
   return (0);
   pid = fork();
   if (pid == -1) 
   {   
   perror("fork");
   return (0);
   }
   if (pid == 0)
   {
   bytes = 0;
   g_exit_status = 0;
   if (!set_signal(HDOC, sig))
   return (0);
   close(pipe_heredoc[READ]);
   expansion = must_be_expanded(lim);
   while (1) 
   {   
   input = readline("> ");
   i++;
   bytes += ft_strlen(input);
   if (bytes > WRITE_MAX)
   {   
   ft_putstr_fd("minishell: heredoc: heredoc too long\n", 2); 
   break ;
   }   
   if (is_eof_heredoc(input, lim, i) || !ft_strcmp(lim, input))
   break ;
   if (expansion)
   {   
   input = heredoc_expansion(input);
   if (!input)
   return (0);
   write(pipe_heredoc[WRITE], input, ft_strlen(input));
   ft_free(input);
   }   
   else
   write(pipe_heredoc[WRITE], input, ft_strlen(input));
   write(pipe_heredoc[WRITE], "\n", 1); 
   }   
   close(pipe_heredoc[WRITE]);
   ft_free(pipe_heredoc);
   ft_free(lim);
   free_tokens_ast(NULL, *list);
   exit_minishell(NULL);
   }   
   waitpid(pid, &status, 0); 
   status = convert_status(status);
   g_exit_status = status;
   clean_heredoc(token, NULL);
   token->heredoc = pipe_heredoc;
   close(pipe_heredoc[WRITE]);
   if (status == 130)
   {
   clean_heredoc(token, NULL);
   return (-2);
   }
   return (1);
   }
   */