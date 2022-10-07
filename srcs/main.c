/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 08:43:23 by charoua           #+#    #+#             */
/*   Updated: 2022/10/07 21:24:01 by agranger         ###   ########.fr       */
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

void	exit_failure(t_node *ast, t_dblist *tokens)
{
	g_exit_status = 1;
	free_tokens_ast(&ast, &tokens);
	exit_minishell(NULL);
}

int	init_minishell(char **envp, t_node **ast, t_dblist **tokens)
{
	int	status;

	*ast = NULL;
	*tokens = NULL;
	status = 1;
	singleton_env(0, &status, envp);
	if (!status)
		return (0);
	return (1);
}

void	launch_cmd(char *input, t_node *ast, t_dblist *tokens, t_sa *sig)
{
	int	error;

	error = 0;
	if (!ft_lexer(input, &tokens, &error, sig))
		exit_failure(ast, tokens);
	if (error || !tokens->first->str)
		return (free_tokens_ast(&ast, &tokens));
	if (!ft_expand(&tokens, &error))
		exit_failure(ast, tokens);
	if (error || !tokens->first->str)
		return (free_tokens_ast(&ast, &tokens));
	if (!parser(&ast, tokens->first, &error))
		exit_failure(ast, tokens);
	if (error)
		return (free_tokens_ast(&ast, &tokens));
	free_tokens_ast(NULL, &tokens);
	if (!error && !exec(ast, sig))
		exit_failure(ast, NULL);
	free_tokens_ast(&ast, NULL);
}

int	main(__attribute__((unused)) int argc,
		__attribute__((unused)) char **argv, char **envp)
{
	char		*input;
	t_node		*ast;
	t_dblist	*tokens;
	t_sa		sig[3];

	if (!init_minishell(envp, &ast, &tokens))
		return (EXIT_FAILURE);
	while (1)
	{
		if (!set_signal(INPUT, sig))
			return (EXIT_FAILURE);
		input = readline("minishell$> ");
		add_history(input);
		is_eof(input);
		if (input && !is_only_spaces(input))
			launch_cmd(input, ast, tokens, sig);
		free(input);
	}
	exit_minishell(NULL);
	return (0);
}
