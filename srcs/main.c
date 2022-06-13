/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 08:43:23 by charoua           #+#    #+#             */
/*   Updated: 2022/06/13 17:56:51 by charoua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sh	*init_sh(void)
{
	t_sh	*minishell;

	minishell = malloc(sizeof(*minishell));
	if (!minishell)
		return (NULL);
	minishell->ast = NULL;
	minishell->tokens = NULL;
	minishell->ret = 0;
	return (minishell);
}

int	is_only_spaces(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_sh		*minishell;

	(void)argc;
	(void)argv;
	(void)envp;
	minishell = init_sh();
	while (1)
	{
		input = readline("minishell$> ");
		add_history(input);
		is_eof(input, minishell);
		if (input && !is_only_spaces(input))
		{
			ft_lexer(input, &(minishell->tokens));
			parser(&(minishell->ast), minishell->tokens->first);
			//print_ast(ast);
			ft_free(&minishell->tokens);
			free(minishell->tokens);
			minishell->tokens = NULL;
			ast_delete_nodes(minishell->ast);
			minishell->ast = NULL;
		}
		free(input);
	}
	rl_clear_history();
	exit_minishell(minishell);
	return (0);
}
