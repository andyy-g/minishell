/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 09:05:09 by charoua           #+#    #+#             */
/*   Updated: 2022/10/07 23:14:38 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_node *node)
{
	char	*cwd;

	(void)node;
	cwd = (char *)malloc(sizeof(char) * PATH_MAX);
	if (cwd && getcwd(cwd, PATH_MAX))
	{
		printf("%s\n", cwd);
		g_exit_status = 0;
	}
	else
		g_exit_status = 1;
	free(cwd);
	return (1);
}
