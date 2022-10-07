/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display_error_bis.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 20:25:36 by charoua           #+#    #+#             */
/*   Updated: 2022/10/07 20:25:38 by charoua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_display_cd(t_err err, char *arg)
{
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
	if (err == ERR_CD_OLDPWD)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set", 2);
		g_exit_status = 1;
	}
}

void	ft_display_builtin(t_err err, char *arg)
{
	if (err == ERR_ENV_NBARG)
	{
		ft_putstr_fd("minishell: env: too many arguments", 2);
		g_exit_status = 1;
	}
	if (err == ERR_UNSET_ARG)
	{
		ft_putstr_fd("minishell: unset: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("' : not a valid identifier", 2);
		g_exit_status = 1;
	}
}

void	ft_display_export(t_err err, char *arg)
{
	if (err == ERR_EXPORT_ARG)
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("' : not a valid identifier", 2);
		g_exit_status = 1;
	}
	if (err == ERR_EXPORT_INV)
	{
		ft_putstr_fd("minishell: export: ", 2);
		ft_putchar_fd(arg[0], 2);
		ft_putchar_fd(arg[1], 2);
		ft_putstr_fd(" : invalid option", 2);
		g_exit_status = 2;
	}
	if (err == ERR_EXPORT_EVENT)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(" : event not found", 2);
		g_exit_status = 2;
	}
}
