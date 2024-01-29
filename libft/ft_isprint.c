/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 09:10:34 by miheider          #+#    #+#             */
/*   Updated: 2023/09/15 10:26:37 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (16384);
	return (0);
}

/*
#include <ctype.h>
#include <stdio.h>
int main(void)
{
	char	a;

	a = '-';

	printf("%d", ft_isprint(a));
	return (0);
}*/
