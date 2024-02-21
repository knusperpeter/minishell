/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 12:57:00 by codespace         #+#    #+#             */
/*   Updated: 2024/01/18 21:33:58 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
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
		return (0);
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	new = malloc(sizeof(char) *(i + j + 1));
	if (new == 0)
		return (0);
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

char	*ft_strchr(const char *s, int c)
{
	int	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if (s[i] == (char) c)
			return ((char *)(s + i));
		i++;
	}
	if (s[i] == (char) c)
		return ((char *)(s + i));
	return (NULL);
}

char	*ft_strdup(const char *s)
{
	char	*dest;
	char	*buffer;

	buffer = (char *)malloc((ft_strlen((char *)s) + 1) * sizeof(char));
	if (!buffer)
		return (0);
	dest = buffer;
	while (*s)
	{
		*buffer = *s;
		s++;
		buffer++;
	}
	*buffer = 0;
	return (dest);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	strlen;
	char	*p;

	if (s == 0)
		return (0);
	strlen = ft_strlen(s);
	if (start > strlen)
		len = 0;
	else if (len > (strlen - start))
		len = strlen - start;
	p = (char *)malloc(sizeof(char) * (len + 1));
	if (p == 0)
		return (0);
	i = 0;
	while (len > 0)
	{
		p[i] = s[start + i];
		i++;
		len--;
	}
	p[i] = 0;
	return (p);
}
