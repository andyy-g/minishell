/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_variable_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charoua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 13:53:34 by charoua           #+#    #+#             */
/*   Updated: 2022/10/09 07:15:48 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
