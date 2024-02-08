/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 14:50:09 by caigner           #+#    #+#             */
/*   Updated: 2023/09/15 10:57:25 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	i;

	if (size == 0)
		return (ft_strlen(src));
	dest_len = 0;
	src_len = 0;
	while (dest[dest_len] != 0 && dest_len < size)
		dest_len++;
	while (src[src_len] != 0)
		src_len++;
	if (size <= dest_len)
		return (dest_len + src_len);
	i = 0;
	while (src[i] != 0 && dest_len + i < size - 1)
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = 0;
	return (dest_len + src_len);
}
