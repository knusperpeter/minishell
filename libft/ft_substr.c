/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:14:15 by miheider          #+#    #+#             */
/*   Updated: 2023/09/15 10:52:55 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*reserved;

	if (s == NULL)
		return (NULL);
	if (len <= 0 || start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	reserved = (char *)malloc(sizeof(char) * (len + 1));
	if (NULL == reserved)
		return (NULL);
	i = 0;
	while (i < len && s[i] != 0)
	{
		reserved[i] = s[start];
		i++;
		start++;
	}
	reserved[i] = '\0';
	return (reserved);
}

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(void)
{
	char	text[] = "Was soll das sein bitte?";
	int		st = 20;
	int		len = 0;
	printf("%s", ft_substr(text, st, len));
	return (0);
}*/
