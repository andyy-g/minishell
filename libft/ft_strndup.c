/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 16:11:50 by agranger          #+#    #+#             */
/*   Updated: 2022/06/17 16:28:24 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	char	*s2;
	int		i;

	if (!s1 || !n)
		return (NULL);
	s2 = malloc(sizeof(char) * (n + 1));
	if (!s2)
		return (NULL);
	i = 0;
	while (s1[i] && i < (int)n)
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
