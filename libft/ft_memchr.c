/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:39:14 by miheider          #+#    #+#             */
/*   Updated: 2023/09/15 10:22:46 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;
	char	*str;

	str = (char *)s;
	i = 0;
	while (i < n)
	{
		if (str[i] == (char)c)
		{
			return (&str[i]);
		}
		i++;
	}
	return (NULL);
}

/*
#include <string.h>
#include <stdio.h>
int main(void)
{
	char	a[] = "Was geht?";
	int		b = 'h';
	size_t	c = 12;
	char	*d;

	d = ft_memchr(a, b, c);
	printf("%s", d);


	return (0);


}*/
