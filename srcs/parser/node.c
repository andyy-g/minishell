/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 17:50:23 by agranger          #+#    #+#             */
/*   Updated: 2022/06/23 18:04:21 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_size_cmd(t_pars *cmd)
{
	int		i;

	i = 0;
	while (cmd->str && (cmd->token == WORD
		|| cmd->token == FD || cmd->token == LIMITOR))
	{
		cmd = cmd->next;
		i++;
	}
	return (i);
}

char	**create_simple_cmd(t_pars **cmd, int *status)
{
	char	**ret;
	int		i;

	*status = 1;
	if (!cmd)
		return (NULL);
	ret = malloc(sizeof(char *) * (get_size_cmd(*cmd) + 1));
	if (!ret)
	{
		*status = 0;
		return (NULL);
	}
	i = 0;
	while ((*cmd)->str && ((*cmd)->token == WORD
		|| (*cmd)->token == FD || (*cmd)->token == LIMITOR))
	{
		ret[i] = ft_strdup((*cmd)->str);
		if (!ret[i])
		{
			*status = 0;
			return (ret);
		}
		i++;
		*cmd = (*cmd)->next;
	}
	ret[i] = NULL;
	return (ret);
}

t_node	*ast_create_node(t_toktype type, t_pars **simple_cmd)
{
	t_node	*ret;
	int		status;

	ret = malloc(sizeof(*ret));
	if (!ret)
		return (NULL);
	ret->type = type;
	ret->cmd = create_simple_cmd(simple_cmd, &status);
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

void	ast_add_children(t_node *parent, t_node *left_child, t_node *right_child)
{
	parent->left = left_child;
	parent->right = right_child;
	left_child->parent = parent;
	right_child->parent = parent;
}

void	print_ast(t_node *ast)
{
	t_node	*curr;
	int		i;
	char	*node_type[12] = { "WORD", "FD", "LIMITOR", "PIPE", "FILE_IN", "HEREDOC", "FILE_OUT", "FILE_OUT_APP", "AND", "OR", "LPAR", "RPAR" };

	curr = ast;
	i = 0;
	printf("ROOT = %s\n", node_type[curr->type]);
	while (curr->cmd && curr->cmd[i])
	{
		printf("	%s\n", curr->cmd[i]);
		i++;
	}
	if (curr->left)
	{
		i = 0;
		printf("LEFT = %s\n", node_type[curr->left->type]);
		while (curr->left->cmd && curr->left->cmd[i])
		{
			printf("	%s\n", curr->left->cmd[i]);
			i++;
		}
	}
	if (curr->right)
	{
		i = 0;
		printf("RIGHT = %s\n", node_type[curr->right->type]);
		while (curr->right->cmd && curr->right->cmd[i])
		{
			printf("	%s\n", curr->right->cmd[i]);
			i++;
		}
	}
}
