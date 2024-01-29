/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 14:04:14 by miheider          #+#    #+#             */
/*   Updated: 2023/09/15 10:46:05 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			x;
	unsigned char	*b;

	b = (unsigned char *)s;
	x = 0;
	while (x < n)
	{
		b[x] = (unsigned char)c;
		x++;
	}
	return (s);
}

/*
#include <string.h>
#include <stdio.h>
int main(void)
{
	char str[] = "AT41 2548 9651 2365 7456";
	int	c = 'x';
	size_t n = 15;

	printf("%s", (unsigned char *)ft_memset(str, c, n));
	return (0);
}*/
