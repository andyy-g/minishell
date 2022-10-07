/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 19:23:56 by agranger          #+#    #+#             */
/*   Updated: 2022/10/07 20:31:31 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_signals_exec(char *cmd, t_sa *sig)
{
	if (!ft_strcmp(cmd, "./minishell"))
	{
		if (!set_signal(IGN, sig))
			return (0);
	}
	else
	{
		if (!set_signal(EXEC, sig))
			return (0);
	}
	return (1);
}
