/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 09:51:24 by caigner           #+#    #+#             */
/*   Updated: 2023/09/13 13:58:56 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check(const char c, const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != 0)
	{
		if (c == str[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	i;
	char	*p;

	if (s1 == 0)
		return (0);
	end = ft_strlen(s1);
	start = 0;
	while (s1[start] && check(s1[start], set))
		start++;
	while (end > start && check(s1[end - 1], set))
		end--;
	p = (char *)malloc((sizeof(*s1)) * (end - start + 1));
	if (p == 0)
		return (0);
	i = 0;
	while (start < end)
	{
		p[i] = s1[start];
		i++;
		start++;
	}
	p[i] = 0;
	return (p);
}
