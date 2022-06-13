/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 08:43:23 by charoua           #+#    #+#             */
/*   Updated: 2022/06/13 12:06:32 by agranger         ###   ########.fr       */
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


int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_dblist	*tokens;
	t_node		*ast;

	(void)argc;
	(void)argv;
	(void)envp;
	ast = NULL;
	tokens = NULL;
	while (1)
	{
		input = readline("minishell$> ");
		add_history(input);
		if (input)
		{
			ft_lexer(input, &tokens);
			parser(&ast, tokens->first);
			//print_ast(ast);
			ft_free(&tokens);
			free(tokens);
			ast_delete_nodes(ast);
			free(input);
		}
	}
	rl_clear_history();
	return (0);
}
