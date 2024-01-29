/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 10:11:41 by miheider          #+#    #+#             */
/*   Updated: 2023/09/15 10:30:10 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		f(i, s + i);
		i++;
	}
}
/*
void	ft_putchar(unsigned int i, char *s)
{
	write(1, &s[i], 1);
}


#include <string.h>
#include <stdio.h>
#include <unistd.h>
int main(void)
{
	char	str[] = "das ist doof";

	ft_striteri(str, ft_putchar);
	return (0);
}*/
