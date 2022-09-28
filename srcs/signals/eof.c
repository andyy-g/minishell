/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eof.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 13:47:48 by agranger          #+#    #+#             */
/*   Updated: 2022/09/28 18:17:52 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_eof(char *input)
{
	if (!input)
	{
		printf("exit\n");
		exit_minishell(NULL);
	}
}

int	is_eof_heredoc(char *input, char *lim, int line)
{
	char	*num_line;

	if (!input)
	{
		ft_putstr_fd("minishell: warning: here-document at line ", 2);
		num_line = ft_itoa(line);
		if (!num_line)
			return (-1);
		ft_putstr_fd(num_line, 2);
		ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(lim, 2);
		ft_putstr_fd("')\n", 2);
		ft_free(num_line);
		return (1);
	}
	return (0);
}
