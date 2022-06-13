/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eof.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 13:47:48 by agranger          #+#    #+#             */
/*   Updated: 2022/06/13 13:59:59 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_eof(char *input, t_sh *minishell)
{
	if (!input)
	{
		printf("exit\n");
		exit_minishell(minishell);
	}
}
