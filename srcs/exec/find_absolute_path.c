/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_absolute_path.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 11:21:35 by agranger          #+#    #+#             */
/*   Updated: 2022/09/26 16:24:27 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_envpath_value(void)
{
	t_env	*env;
	char	**paths;

	env = singleton_env(1, NULL, NULL);
	while (env && ft_strcmp(env->var, "PATH"))
		env = env->next;
	if (!env)
		return (NULL);
	paths = ft_split(env->value, ':');
	return (paths);
}

char	*concat_pathname(char *path, char *cmd)
{
	int		size;
	char	*pathname;

	size = ft_strlen(path) + ft_strlen(cmd) + 1;
	pathname = ft_calloc((size + 1), sizeof(char));
	ft_strcpy(pathname, path);
	ft_strcat(pathname, "/");
	ft_strcat(pathname, cmd);
	return (pathname);
}

int	no_env_or_cmd_empty(int *cmd_not_found, char **paths)
{
	*cmd_not_found = 1;
	free_arr_of_str(paths);
	return (1);
}

int	find_path_bin(t_node *node, char **pathname, int *cmd_not_found)
{
	char		**paths;
	struct stat	sb;
	int			i;

	sb.st_mode = 0;
	paths = get_envpath_value();
	if (!paths || !node->cmd[0][0] || node->cmd[0][0] == '.')
		return (no_env_or_cmd_empty(cmd_not_found, paths));
	i = -1;
	while (paths[++i])
	{
		*pathname = concat_pathname(paths[i], node->cmd[0]);
		if (!*pathname)
		{
			free_arr_of_str(paths);
			return (0);
		}
		if ((stat(*pathname, &sb) == 0 && S_ISDIR(sb.st_mode) == 0))
			break ;
		ft_free(*pathname);
	}
	if (!paths[i])
		*cmd_not_found = 1;
	free_arr_of_str(paths);
	return (1);
}
