/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 14:24:54 by miheider          #+#    #+#             */
/*   Updated: 2023/09/15 10:37:30 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	int					i;
	const unsigned char	*a;
	const unsigned char	*b;

	a = (unsigned char *)s1;
	b = (unsigned char *)s2;
	i = 0;
	while (i < (int)n)
	{
		if (a[i] != b[i])
			return (a[i] - b[i]);
		i++;
	}
	return (0);
}

/*
#include <string.h>
#include <stdio.h>

int main(void)
{
	char	a[] = "Wieso nur?";
	char	b[] = "Wieso nur?";
	int		i;

	i = 356;

	printf("%d", ft_memcmp(a, b, i));
	printf("\n%d", memcmp(a, b, i));

	return (0);
}*/
