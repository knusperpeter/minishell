/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 19:01:31 by miheider          #+#    #+#             */
/*   Updated: 2024/03/29 16:13:51 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		a;

	a = 0;
	while (s[a])
	{
		if (s[a] == (char)c)
		{
			return ((char *)&s[a]);
		}
		a++;
	}
	if ((char)c == '\0')
		return ((char *)&s[a]);
	return (NULL);
}

/*
#include <stdio.h>

int main(void)
{
	char string[] = "www.wasgehtab.at";
	char c = '#';

	printf("%s", ft_strchr(string, c));
	return (0);
}*/
