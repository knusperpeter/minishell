/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 11:55:23 by caigner           #+#    #+#             */
/*   Updated: 2023/09/12 16:47:00 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (*little == 0)
	{
		return ((char *)big);
	}
	if (big == 0 && len == 0)
		return (0);
	while (big[i] && i < len)
	{
		while (big[i + j] == little[j] && little[j] != 0 && i + j < len)
		{
			j++;
		}
		if (little[j] == 0)
			return ((char *)big + i);
		j = 0;
		i++;
	}
	return (0);
}
