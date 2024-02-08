/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:59:24 by caigner           #+#    #+#             */
/*   Updated: 2023/09/07 14:58:17 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	size_t	i;
	size_t	j;
	char	*p;

	i = 0;
	j = 0;
	while (src[i])
		i++;
	p = (char *)malloc(i + 1);
	if (p == 0)
		return (0);
	while (j < i)
	{
		p[j] = src[j];
		j++;
	}
	p[j] = '\0';
	return (p);
}
