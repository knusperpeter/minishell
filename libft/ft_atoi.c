/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 19:06:42 by miheider          #+#    #+#             */
/*   Updated: 2023/09/15 10:47:56 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	x;
	int	sign;

	sign = 1;
	x = 0;
	i = 0;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] != '\0' && nptr[i] == 45)
	{
		sign = -1;
		i++;
	}
	else if (nptr[i] != '\0' && nptr[i] == 43)
		i++;
	while (nptr[i] != '\0' && (nptr[i] >= 48 && nptr[i] <= 57))
	{
		x = 10 * x + nptr[i] - 48;
		i++;
	}
	return (sign * x);
}
