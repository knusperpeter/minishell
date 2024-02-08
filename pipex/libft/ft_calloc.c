/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:48:37 by caigner           #+#    #+#             */
/*   Updated: 2023/12/04 20:06:43 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			i;
	size_t			total;
	void			*ptr;
	unsigned char	*byteptr;

	i = 0;
	total = nmemb * size;
	if (nmemb != 0 && total / nmemb != size)
		return (0);
	ptr = malloc(total);
	if (ptr != 0)
	{
		byteptr = (unsigned char *)ptr;
		while (i < total)
		{
			byteptr[i] = 0;
			i++;
		}
	}
	return (ptr);
}
