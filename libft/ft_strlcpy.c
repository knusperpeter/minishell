/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 09:51:48 by miheider          #+#    #+#             */
/*   Updated: 2024/02/13 23:55:28 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	x;
	unsigned int	y;

	x = 0;
	while (src && src[x])
		x++;
	if (!size)
		return (x);
	y = 0;
	while (src[y] != '\0' && y < (size -1))
	{
		dst[y] = src[y];
		y++;
	}
	dst[y] = '\0';
	return (x);
}

/*
#include <bsd/string.h>
#include <stdio.h>
int main(void)
{
	char	a[] = "";
	char	b[] = "Naund!";
	size_t	c = 4;

	printf("%zu", ft_strlcpy(b, a, c));
	printf("%s", b);
	printf("%zu", strlcpy(b, a, c));
	printf("%s", b);
	return (0);
}*/
