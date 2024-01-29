/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 18:59:26 by miheider          #+#    #+#             */
/*   Updated: 2023/09/15 10:38:31 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	int		*a;

	a = malloc(nmemb * size);
	if (a == 0)
		return (NULL);
	else
		ft_memset(a, 0, (nmemb * size));
	return (a);
}
