/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:10:49 by caigner           #+#    #+#             */
/*   Updated: 2023/09/13 16:25:50 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	i;
	size_t	len;
	char	*p;

	if (s == 0 || f == 0)
		return (0);
	len = ft_strlen(s);
	p = malloc(sizeof(*s) * (len + 1));
	if (p == 0)
		return (0);
	i = 0;
	while (i < len)
	{
		p[i] = f(i, s[i]);
		i++;
	}
	p[i] = 0;
	return (p);
}
