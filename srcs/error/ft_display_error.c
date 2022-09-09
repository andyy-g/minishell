/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display_error.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 09:28:39 by charoua           #+#    #+#             */
/*   Updated: 2022/09/09 09:28:46 by charoua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_display_three_line(t_err err, char *arg)
{
	if (err == ERR_UNEXPECTED_TOK)
	{
		ft_putstr_fd("syntax error near unexpected token `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("'", 2);
	}
	if (err == ERR_MATCHING_TOK)
	{
		ft_putstr_fd("unexpected EOF while looking for matching `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("'", 2);
	}
}

void	ft_display_two_line(t_err err, char *arg)
{
	if (err == ERR_PERM_DENIED)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": Permission denied", 2);
	}
	if (err == ERR_CMD_NOT_FOUND)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": command not found", 2);
	}
	if (err == ERR_IS_DIR)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": Is a directory", 2);
	}
	if (err == ERR_NO_FILE)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": No such file or directory", 2);
	}
}

void	display_error(t_err err, char *arg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_display_two_line(err, arg);
	ft_display_three_line(err, arg);
	ft_putstr_fd("\n", 2);
}
