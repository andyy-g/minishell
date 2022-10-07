/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 08:43:23 by charoua           #+#    #+#             */
/*   Updated: 2022/10/07 01:36:41 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

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

void	free_tokens_ast(t_node *ast, t_dblist *tokens)
{
	if (tokens)
		ft_free_tokens(tokens);
	if (ast)
		ast_delete_nodes(ast);
}

void	exit_failure(t_node *ast, t_dblist *tokens)
{
	g_exit_status = 1;
	free_tokens_ast(ast, tokens);
	exit_minishell(NULL);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	int			status;
	t_node		*ast;
	t_dblist	*tokens;
	int			error;
	t_sa		sig[3];

	(void)argc;
	(void)argv;
	status = 1;
	singleton_env(0, &status, envp);
	if (!status)
		return (EXIT_FAILURE);
	while (1)
	{
		if (!set_signal(INPUT, sig))
			return (0);
		ast = NULL;
		tokens = NULL;
		input = readline("minishell$> ");
		add_history(input);
		is_eof(input);
		if (input && !is_only_spaces(input))
		{
			error = 0;
			if (!ft_lexer(input, &tokens, &error, sig))
				exit_failure(ast, tokens);
			if (error || !tokens->first->str)
			{
				free_tokens_ast(ast, tokens);
				continue ;
			}
			if (!ft_expand(&tokens, &error))
				exit_failure(ast, tokens);
			if (error || !tokens->first->str)
			{
				free_tokens_ast(ast, tokens);
				continue ;
			}

			if (!parser(&ast, tokens->first, &error))
				exit_failure(ast, tokens);
			if (error)
			{
				free_tokens_ast(ast, tokens);
				continue ;
			}
			free_tokens_ast(NULL, tokens);
			if (!error && !exec(ast, sig))
				exit_failure(ast, NULL);
			free_tokens_ast(ast, NULL);
		}
		free(input);
	}
	exit_minishell(NULL);
	return (0);
}
