#include "libft.h"

static long	ft_length_nb(long n)
{
	long size;

	if (n == 0)
		return (1);
	size = 0;
	if (n < 0)
	{
		size++;
		n = -n;
	}
	while (n != 0)
	{
		n = n / 10;
		size++;
	}
	return (size);
}

char		*ft_itoa(int n)
{
	long i;
	long n_long;
	long length;
	char *new;

	i = 0;
	n_long = n;
	length = ft_length_nb(n_long);
	if ((new = (char *)malloc(sizeof(char) * length + 1)) == NULL)
		return (NULL);
	if (n_long < 0)
	{
		n_long = -n_long;
		new[i++] = '-';
	}
	new[length] = '\0';
	length--;
	while (length >= i)
	{
		new[length] = (n_long % 10) + '0';
		length--;
		n_long = n_long / 10;
	}
	return (new);
}