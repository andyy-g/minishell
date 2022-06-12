/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 22:12:18 by agranger          #+#    #+#             */
/*   Updated: 2022/06/09 22:41:22 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// echo "Hello World" > file_out | ls && (cat < file_out | wc -l || echo "test")

t_node	*parser(t_token *token)
{
	while (token)
	{

		token = token->next;
	}
}

