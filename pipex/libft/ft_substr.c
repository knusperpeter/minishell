/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:12:17 by caigner           #+#    #+#             */
/*   Updated: 2023/09/13 09:44:38 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
