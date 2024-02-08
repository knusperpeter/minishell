/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 18:36:36 by caigner           #+#    #+#             */
/*   Updated: 2023/09/14 10:57:09 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	sign(int n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

static int	getsize(int n)
{
	int	i;

	i = 0;
	if (n <= 0)
		++i;
	while (n != 0)
	{
		++i;
		n /= 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*a;
	int		size;

	size = getsize(n);
	a = (char *)malloc(sizeof(char) * (size + 1));
	if (!a)
		return (0);
	a[size] = 0;
	if (n < 0)
		a[0] = '-';
	else if (n == 0)
		a[0] = '0';
	while (n != 0)
	{
		size--;
		a[size] = sign(n % 10) + '0';
		n /= 10;
	}
	return (a);
}

/*#include <stdio.h>

int main(){
	char *p = ft_itoa(-21474864);
	while (*p)
	{
		printf("%p\n", &p);
		p++;
	}
	return 0;
}
*/