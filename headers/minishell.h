/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 08:46:50 by charoua           #+#    #+#             */
/*   Updated: 2022/06/11 16:19:40 by agranger         ###   ########.fr       */
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
# include <stdbool.h>
# include "../libft/libft.h"

typedef enum e_toktype
{
	WORD,
	PIPE,
	FILE_IN,
	HEREDOC,
	FILE_OUT,
	FILE_OUT_APP,
	AND,
	OR,
	LPAR,
	RPAR
}	t_toktype;

typedef struct s_pars
{
	char			*str;
	int				token;
	int				db_quote;
	int				sp_quote;
	struct s_pars	*prev;
	struct s_pars	*next;
}	t_pars;

typedef struct	s_node
{
	char			*name;
	t_toktype		type;
	bool			exp;
	struct s_node	*parent;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

typedef struct s_dblist
{
	t_pars	*first;
	t_pars	*last;
}	t_dblist;

int		ft_add_pars(char *str, t_pars **pars, t_dblist **list);
t_pars	*ft_create_pars(t_pars *prev);
int		ft_quote(char *str, char c, t_pars **pars);
int		ft_line(char *str, t_pars **pars);
int		ft_input(char *str, t_pars **pars);
int		ft_output(char *str, t_pars **pars);
int		ft_and(char *str, t_pars **pars);
int		ft_word(char *str, t_pars **pars);

#endif
