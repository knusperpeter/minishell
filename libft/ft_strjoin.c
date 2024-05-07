/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:32:00 by miheider          #+#    #+#             */
/*   Updated: 2024/05/07 20:42:21 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*new;

	if (s1 == 0 || s2 == 0)
	{
		if (s1)
			return ((char *)s1);
		return (0);
	}
	i = strlen(s1);
	j = strlen(s2);
	new = malloc(sizeof(char) *(i + j + 1));
	if (new == 0) //causes mem leak if malloc fail
		return (0);
	//if (1)
	//{
	//	dprintf(2, "\n\n\n\n hi \n\n\n");
	//	return(0);
	//}
	i = 0;
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		new[i + j] = s2[j];
		j++;
	}
	new[i + j] = 0;
	return (new);
}
/*
int main(void)
{
	char text1[] = "Warum";
	char text2[] = "machst du das?";
	printf("%s", ft_strjoin(text1, text2));
	return 0;
}*/
