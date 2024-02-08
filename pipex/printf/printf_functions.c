/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:02:08 by caigner           #+#    #+#             */
/*   Updated: 2023/12/14 12:47:58 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(const char c)
{
	write(2, &c, 1);
	return (1);
}

int	ft_putnbr(long long n)
{
	char	digit;
	int		chars;

	chars = 0;
	if (n < 0)
	{
		chars += write (2, "-", 1);
		n = -n;
	}
	if (n >= 10)
		chars += ft_putnbr(n / 10);
	digit = '0' + (n % 10);
	chars += write (2, &digit, 1);
	return (chars);
}

int	ft_putstr(const char *s)
{
	int	i;

	i = 0;
	if (!s)
	{
		i += ft_putstr("(null)");
		return (i);
	}
	while (s[i])
	{
		write(2, &s[i], 1);
		i++;
	}
	return (i);
}

int	ft_putptr(unsigned long int nbr)
{
	int	chars;

	chars = 0;
	if (nbr == 0)
		chars += ft_putstr("(nil)");
	else
	{
		chars += ft_putstr("0x");
		chars += ft_putnbrptr(nbr);
	}
	return (chars);
}

int	ft_puthexnbr(unsigned int nbr, const char capital)
{
	int					result;
	int					chars;
	char				*base;

	chars = 0;
	base = "0123456789abcdef";
	if (capital == 'X')
		base = "0123456789ABCDEF";
	if (nbr >= 16)
		chars += ft_puthexnbr(nbr / 16, capital);
	result = base[nbr % 16];
	chars += write (2, &result, 1);
	return (chars);
}
