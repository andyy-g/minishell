/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_charset_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 14:15:31 by agranger          #+#    #+#             */
/*   Updated: 2022/09/27 14:15:55 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_charset_str(char *str, char *charset)
{
	int	i;
	int	j;

	i = 0;
	if (!str[i] || !str)
		return (0);
	while (charset[i])
	{
		j = 0;
		while (str[j])
		{
			if (charset[i] == str[j])
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}
