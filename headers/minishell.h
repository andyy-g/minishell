/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarasso <tcarasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 14:34:29 by tcarasso          #+#    #+#             */
/*   Updated: 2022/06/10 10:14:44 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <signal.h>
# include <stddef.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>
# include "../libft/libft.h"

typedef struct	s_node	
{
	char			*name;
	t_toktype		type;
	bool			exp;
	struct s_node	*parent;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

#endif
