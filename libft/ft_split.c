/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 21:58:02 by agranger          #+#    #+#             */
/*   Updated: 2022/10/09 07:13:17 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	word_count(char	*str, char c)
{
	int	i;
	int	nb_word;

	i = 0;
	nb_word = 0;
	while (str[i])
	{
		if (str[i] && str[i] != c)
		{
			nb_word++;
			while (str[i] && str[i] != c)
				i++;
		}
		else
			i++;
	}
	return (nb_word);
}

int	word_len(char **str, char c)
{
	int	len;

	while (**str && **str == c)
		(*str)++;
	len = 0;
	while ((*str)[len] && (*str)[len] != c)
		len++;
	return (len);
}

char	**ft_split(char *str, char c)
{
	char	**ret;
	int		nb_word;
	int		i;
	int		j;

	nb_word = word_count(str, c);
	ret = malloc(sizeof(char *) * (nb_word + 1));
	i = 0;
	while (i < nb_word)
	{
		j = 0;
		ret[i] = malloc(sizeof(char) * (word_len(&str, c) + 1));
		while (*str && *str != c)
		{
			ret[i][j] = *str;
			j++;
			str++;
		}
		ret[i][j] = '\0';
		str++;
		i++;
	}
	ret[i] = NULL;
	return (ret);
}
