/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agranger <agranger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 19:29:42 by agranger          #+#    #+#             */
/*   Updated: 2022/06/30 16:49:44 by agranger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>

size_t	ft_strlen(const char *str);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_tolower(int c);
char	*ft_strdup(const char *s1);
char	*ft_strndup(const char *s1, size_t n);
char	*ft_strcpy(char *dst, char *src);
char	*ft_strcat(char *dst, char *src);
char	**ft_split(char *str, char c);
char	*ft_itoa(int n);
void	ft_free(void *elem);
void	free_arr_of_str(char **arr);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);

#endif
