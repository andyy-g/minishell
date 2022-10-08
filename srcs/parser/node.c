/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 17:50:23 by agranger          #+#    #+#             */
/*   Updated: 2022/10/08 16:58:09 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_size_cmd(t_pars *cmd, t_toktype type)
{
	int		i;

	if (type != WORD)
		return (1);
	i = 0;
	while (cmd->str && cmd->token == WORD)
	{
		cmd = cmd->next;
		i++;
	}
	while (cmd->str && is_redir_token(cmd))
		cmd = cmd->next;
	while (cmd->str && cmd->token == WORD)
	{
		cmd = cmd->next;
		i++;
	}
	return (i);
}

char	**create_simple_cmd(t_pars **cmd, int *status, t_toktype type)
{
	char	**ret;
	int		i;
	int		size;

	*status = 1;
	if (!cmd)
		return (NULL);
	size = get_size_cmd(*cmd, type);
	ret = malloc(sizeof(char *) * (size + 1));
	if (!ret)
		return (set_status_error(status, NULL));
	i = 0;
	while ((*cmd)->str && (*cmd)->token == (int)type)
	{
		ret[i] = ft_strdup((*cmd)->str);
		if (!ret[i])
			return (set_status_error(status, ret));
		i++;
		*cmd = (*cmd)->next;
		if ((*cmd)->token != WORD)
			break ;
	}
	ret[i] = NULL;
	return (ret);
}

t_node	*ast_create_node(t_toktype type, t_pars **simple_cmd)
{
	t_node	*ret;
	int		status;

	status = 1;
	ret = malloc(sizeof(*ret));
	if (!ret)
		return (NULL);
	ret->type = type;
	ret->heredoc = (*simple_cmd)->heredoc;
	ret->cmd = create_simple_cmd(simple_cmd, &status, type);
	ret->fd_in = 0;
	ret->fd_out = 1;
	ret->is_pipe = false;
	ret->parent = NULL;
	ret->left = NULL;
	ret->right = NULL;
	if (!status)
	{
		ast_delete_nodes(ret);
		return (NULL);
	}
	return (ret);
}

void	ast_delete_nodes(t_node *node)
{
	int	i;

	if (!node)
		return ;
	if (node->cmd)
	{
		i = 0;
		while (node->cmd[i])
		{
			free(node->cmd[i]);
			node->cmd[i] = NULL;
			i++;
		}
		free(node->cmd);
		node->cmd = NULL;
	}
	ast_delete_nodes(node->left);
	ast_delete_nodes(node->right);
	free(node);
	node = NULL;
}

t_node	*ast_add_children(t_node *parent, t_node *left_child,
	t_node *right_child)
{
	parent->left = left_child;
	parent->right = right_child;
	if (left_child)
		left_child->parent = parent;
	right_child->parent = parent;
	return (parent);
}
