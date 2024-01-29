/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:32:00 by miheider          #+#    #+#             */
/*   Updated: 2023/09/05 16:57:26 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*test(char const *s1, char const *s2)
{
	if (!s1 && !s2)
		return (NULL);
	if (s1 && !s2)
		return (ft_strdup(s1));
	if (!s1 && s2)
		return (ft_strdup(s2));
	return (0);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	i;
	unsigned int	j;
	char			*reserved;

	test(s1, s2);
	reserved = malloc(sizeof(char) * ((ft_strlen(s1) + ft_strlen(s2)) + 1));
	if (NULL == reserved)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		reserved[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		reserved[i] = s2[j];
		i++;
		j++;
	}
	reserved[i] = '\0';
	return (reserved);
}
/*
int main(void)
{
	char text1[] = "Warum";
	char text2[] = "machst du das?";
	printf("%s", ft_strjoin(text1, text2));
	return 0;
}*/
