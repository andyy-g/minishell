/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 08:43:23 by charoua           #+#    #+#             */
/*   Updated: 2022/06/13 15:12:38 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free(t_dblist **list)
{
	t_pars	*tmp;
	t_pars	*to_free;

	to_free = (*list)->first;
	while (to_free)
	{
		tmp = to_free->next;
		if (to_free->str)
			free(to_free->str);
		if (to_free)
			free(to_free);
		to_free = tmp;
	}
}

t_pars	*ft_create_pars(t_pars *prev)
{
	t_pars	*pars;

	pars = (t_pars *)malloc(sizeof(t_pars));
	if (pars)
	{
		pars->str = NULL;
		pars->db_quote = 0;
		pars->sp_quote = 0;
		if (prev)
			pars->prev = prev;
		else
			pars->prev = NULL;
		pars->next = NULL;
	}
	else
		return (NULL);
	return (pars);
}

int	ft_lexer(char *str, t_dblist **list)
{
	int	error;
	t_pars		*pars;

	error = 0;
	*list = (t_dblist *)malloc(sizeof(t_dblist));
	pars = ft_create_pars(NULL);
	if ((list) && (pars))
	{
		(*list)->first = pars;
		(*list)->last = pars;
		error = ft_add_lex(str, &pars, list);
		if (error != 1)
			printf("Error : %d\n", error);
//		ft_error(i);
		return (1);
	}
	return (0);
}

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
