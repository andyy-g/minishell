/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display_error.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 09:28:39 by charoua           #+#    #+#             */
/*   Updated: 2022/09/26 17:17:18 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_display_builtin(t_err err, char *arg)
{
	if (err == ERR_ENV_NBARG)
	{
		ft_putstr_fd("minishell: env: too many arguments", 2);
		g_exit_status = 1;
	}
	if (err == ERR_UNSET_ARG)
	{
		ft_putstr_fd("minishell: unset: '", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("' : not a valid identifier", 2);
		g_exit_status = 1;
	}
	if (err == ERR_CD_NBARG)
	{
		ft_putstr_fd("minishell: cd: too many arguments", 2);
		g_exit_status = 1;
	}
	if (err == ERR_CD_CHDIR)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		g_exit_status = 1;
	}
	if (err == ERR_CD_HOME)
	{
		ft_putstr_fd("minishell: cd: HOME not set", 2);
		g_exit_status = 1;
	}
}

void	ft_display_exit(t_err err, char *arg)
{
	if (err == ERR_EXIT_NBARG)
	{
		ft_putstr_fd("minishell: exit: too many arguments", 2);
		g_exit_status = 1;
	}
	if (err == ERR_EXIT_ARG_NO_NUM)
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required", 2);
		g_exit_status = 2;
	}
}

void	ft_display_three_line(t_err err, char *arg)
{
	if (err == ERR_UNEXPECTED_TOK)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("'", 2);
	}
	if (err == ERR_MATCHING_TOK)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("unexpected EOF while looking for matching `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("'", 2);
	}
	if (err == ERR_AMB_REDIRECT)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ambiguous redirect", 2);
	}
}

void	ft_display_two_line(t_err err, char *arg)
{
	if (err == ERR_PERM_DENIED || err == ERR_CMD_NOT_FOUND \
	|| err == ERR_IS_DIR || err == ERR_NO_FILE)
		ft_putstr_fd("minishell: ", 2);
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
	ft_display_two_line(err, arg);
	ft_display_three_line(err, arg);
	ft_display_exit(err, arg);
	ft_display_builtin(err, arg);
	ft_putstr_fd("\n", 2);
}
