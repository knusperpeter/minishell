/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:34:14 by miheider          #+#    #+#             */
/*   Updated: 2023/09/15 10:43:07 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			ctr;
	unsigned char	*d;
	unsigned char	*s;

	if (dest == NULL && src == NULL)
		return (NULL);
	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	ctr = 0;
	while (ctr < n)
	{
		*d = *s;
		ctr++;
		s++;
		d++;
	}
	return (d - n);
}

/*
#include <string.h>
#include <stdio.h>
int main(void)
{
	char a[] = "Was ist nur los mit dir?";
	char b[] = "Oida, das noch so spannend!";

	printf("%s", (char *)ft_memcpy(b, a, 7));
	
	return (0);
}*/
