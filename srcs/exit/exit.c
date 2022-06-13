/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 14:03:30 by agranger          #+#    #+#             */
/*   Updated: 2022/06/13 14:11:48 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_minishell(t_sh *minishell)
{
	int	ret;

	if (minishell)
	{
		ret = minishell->ret;
		if (minishell->ast)
			ast_delete_nodes(minishell->ast);
		if (minishell->tokens)
		{
			ft_free(&(minishell->tokens));
			free(minishell->tokens);
		}
		free(minishell);
	}
	exit(ret);
}
