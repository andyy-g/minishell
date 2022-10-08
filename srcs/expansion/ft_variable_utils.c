/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_variable_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 13:53:34 by charoua           #+#    #+#             */
/*   Updated: 2022/10/08 13:53:36 by charoua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_concat(char **str, int i, int j, int word)
{
	int	k;

	k = 0;
	if (word == 1 && (*str)[i + k])
	{
		(*str)[i] = ' ';
		i++;
	}
	while ((*str)[j + k] && (*str)[i + k])
	{
		(*str)[i + k] = (*str)[j + k];
		k++;
	}
	(*str)[i + k] = '\0';
}

int	ft_trim_replace(char **str, int i, int word)
{
	int	j;

	if ((*str)[i] == ' ' || (*str)[i] == '\n' || (*str)[i] == '\n')
	{
		j = i;
		while ((*str)[j] == ' ' || (*str)[j] == '\n' || (*str)[j] == '\n')
			j++;
		if ((*str)[j] == '\0')
		{
			(*str)[i] = '\0';
			return (-2);
		}
		else if (word == 0)
		{
			ft_concat(&(*str), i, j, word);
			i--;
		}
		else if (word == 1)
			ft_concat(&(*str), i, j, word);
	}
	return (i);
}

void	ft_trim_str(char **str)
{
	int	i;
	int	word;

	i = 0;
	word = 0;
	while ((*str)[i])
	{
		word = 0;
		while ((*str)[i] && (*str)[i] != ' '
			&& (*str)[i] != '\n' && (*str)[i] != '\n')
		{
			word = 1;
			i++;
		}
		i = ft_trim_replace(&(*str), i, word);
		if (i == -2)
			break ;
		i++;
	}
}

char	*ft_fill_new(char *str, char *var, int start, int end)
{
	int		i;
	int		j;
	int		size;
	char	*new;

	i = 0;
	j = 0;
	size = ft_strlen(var) + ft_strlen(str) - end + start;
	new = (char *)malloc(sizeof(char) * size + 1);
	if (new)
	{
		while (str[i] && i < start)
		{
			new[i] = str[i];
			i++;
		}
		while (var[j])
			new[i++] = var[j++];
		j = 0;
		while (str[end + j])
			new[i++] = str[end + j++];
		new[i] = '\0';
	}
	return (new);
}
