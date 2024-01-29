/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 11:55:31 by miheider          #+#    #+#             */
/*   Updated: 2023/09/15 10:24:10 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
		return (c - 32);
	return (c);
}

/*
#include <ctype.h>
#include <stdio.h>
int main(void)
{
	char b;

	b = '5';
	ft_toupper(b);
	printf("%c", toupper(b));
	return (0);
}*/
