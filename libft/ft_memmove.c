/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 16:12:25 by miheider          #+#    #+#             */
/*   Updated: 2023/09/15 11:13:12 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	if (dest == NULL && src == NULL)
		return (dest);
	if (s < d)
	{
		while (n--)
		{
			d[n] = s[n];
		}
	}
	else
	{
		while (n > 0)
		{
			*d++ = *s++;
			n--;
		}
	}
	return (dest);
}

/*
#include <string.h>
#include <stdio.h>
int main(void)
{
	char	b[] = "556754";
	char 	a[] = "isdgcshsa";
	int		c = 15;

	ft_memmove(b, a, c);
//	memmove(b, a, c);
	printf("%s", b);
	return (0);
}*/
