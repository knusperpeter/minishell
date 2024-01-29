/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 09:51:29 by miheider          #+#    #+#             */
/*   Updated: 2023/09/23 10:17:09 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int				a;
	unsigned char	*str;

	str = (unsigned char *)s;
	a = 0;
	while (str[a])
		a++;
	while (a >= 0)
	{
		if (str[a] == (unsigned char)c)
		{
			return ((char *)&str[a]);
		}
		a--;
	}
	return (0);
}

/*
#include <stdio.h>
#include <string.h>
int main(void)
{
	char str[] = "xwww.wassolldas.at";
	int c = 'd';

	printf("%s", ft_strrchr(str, c));
	return (0);
}*/
