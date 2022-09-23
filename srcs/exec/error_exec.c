/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 11:44:50 by agranger          #+#    #+#             */
/*   Updated: 2022/09/23 11:46:01 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_error(t_node *node, int is_dir, int cmd_not_found)
{
	if (is_dir)
	{
		display_error(ERR_IS_DIR, node->cmd[0]);
		return (false);
	}
	if (cmd_not_found)
	{
		display_error(ERR_CMD_NOT_FOUND, node->cmd[0]);
		return (false);
	}
	return (true);
}

void	which_error(t_node *node)
{
	if (errno == EACCES)
		display_error(ERR_PERM_DENIED, node->cmd[0]);
	if (errno == ENOENT)
		display_error(ERR_NO_FILE, node->cmd[0]);
}
