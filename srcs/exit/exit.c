/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 14:03:30 by agranger          #+#    #+#             */
/*   Updated: 2022/09/11 17:29:15 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_minishell(t_node *ast)
{
	while (ast && ast->parent)
		ast = ast->parent;
	rl_clear_history();
	free_tokens_ast(&ast, NULL);
	singleton_env(2, NULL, NULL);
	exit(g_exit_status);
}
