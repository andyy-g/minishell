/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiguous_redirect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 15:50:26 by agranger          #+#    #+#             */
/*   Updated: 2022/10/09 06:26:02 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_ambiguous_redirect(t_pars **exp,
t_env *env, char *str, int *error)
{
	if ((*exp)->prev && (is_chevron((*exp)->prev->token)
			&& (*exp)->prev->token != HEREDOC)
		&& is_charset_str(env->value, " \t\n"))
	{
		display_error(ERR_AMB_REDIRECT, str);
		*error = 1;
		return (1);
	}
	return (0);
}
