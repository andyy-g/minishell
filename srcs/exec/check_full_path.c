/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_full_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 11:21:13 by agranger          #+#    #+#             */
/*   Updated: 2022/09/23 11:34:34 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	contain_slash(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (true);
		i++;
	}
	return (false);
}

void	check_full_path(t_node *node, char **pathname,
		int *is_dir, int *cmd_not_found)
{
	struct stat	sb;
	char		**paths;

	paths = get_envpath_value();
	sb.st_mode = 0;
	stat(node->cmd[0], &sb);
	if (S_ISDIR(sb.st_mode))
	{
		*is_dir = 1;
		free_arr_of_str(paths);
		return ;
	}
	if (paths && !contain_slash(node->cmd[0]))
	{
		*cmd_not_found = 1;
		free_arr_of_str(paths);
		return ;
	}
	free_arr_of_str(paths);
	*cmd_not_found = 0;
	*pathname = ft_strdup(node->cmd[0]);
}
