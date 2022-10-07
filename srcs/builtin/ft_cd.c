/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 14:22:22 by charoua           #+#    #+#             */
/*   Updated: 2022/10/07 23:11:26 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_go_path_oldpwd(char *env_path)
{
	if (!env_path)
		display_error(ERR_CD_OLDPWD, NULL);
	else
	{
		printf("%s\n", env_path);
		ft_update_pwd("OLDPWD", 1);
	}
}

void	ft_go_path(int option)
{
	t_env	*env;
	char	*env_path;

	env_path = NULL;
	env = singleton_env(1, NULL, NULL);
	if (option == 0)
	{
		ft_update_pwd("OLDPWD", 0);
		env_path = get_env_path(env, "HOME");
		if (!env_path)
			display_error(ERR_CD_HOME, NULL);
	}
	else if (option == 1)
	{
		env_path = get_env_path(env, "OLDPWD");
		ft_go_path_oldpwd(env_path);
	}
	if (env_path)
	{
		if ((option == 1 && ft_strcmp("\0", env_path)) || option == 0)
			g_exit_status = chdir(env_path);
		free(env_path);
	}
}

int	ft_cd(t_node *node)
{
	if (node && node->cmd[1] && node->cmd[2])
		display_error(ERR_CD_NBARG, node->cmd[1]);
	else if (node)
	{
		if (!(node->cmd[1]))
			ft_go_path(0);
		else if (node->cmd[1] && ft_strcmp(node->cmd[1], "-") == 0)
			ft_go_path(1);
		else
		{
			ft_update_pwd("OLDPWD", 1);
			g_exit_status = -chdir(node->cmd[1]);
			ft_update_pwd("PWD", 1);
			if (g_exit_status != 0)
				display_error(ERR_CD_CHDIR, node->cmd[1]);
		}
		if (g_exit_status == 0)
			ft_update_pwd("PWD", 1);
	}
	return (1);
}
