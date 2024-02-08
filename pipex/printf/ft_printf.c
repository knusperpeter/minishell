/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 13:53:00 by caigner           #+#    #+#             */
/*   Updated: 2023/12/14 12:47:21 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	formatcheck(const char c, va_list args)
{
	int	result;

	result = 0;
	if (c == 'c')
		result += ft_putchar(va_arg(args, int));
	else if (c == 's')
		result += ft_putstr(va_arg(args, char *));
	else if (c == 'p')
		result += ft_putptr(va_arg(args, unsigned long int));
	else if (c == 'd' || c == 'i')
		result += ft_putnbr(va_arg(args, int));
	else if (c == 'u')
		result += ft_unsignednbr(va_arg(args, unsigned int));
	else if (c == 'x')
		result += ft_puthexnbr(va_arg(args, unsigned int), c);
	else if (c == 'X')
		result += ft_puthexnbr(va_arg(args, unsigned int), c);
	else if (c == '%')
		result += ft_putchar('%');
	return (result);
}

int	findchar(char c)
{
	char	*s;

	s = "cspdiuxX%";
	while (*s != '\0')
	{
		if (*s == c)
			return (1);
		s++;
	}
	return (0);
}

int	print(const char *format, va_list args)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			if (findchar(format[i + 1]) == 1)
				result += formatcheck(format[i + 1], args);
			else
				return (result);
			i++;
		}
		else
		{
			ft_putchar(format[i]);
			result++;
		}
		i++;
	}
	return (result);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		result;

	if (!format)
		return (-1);
	va_start (args, format);
	result = print(format, args);
	va_end(args);
	return (result);
}

/* #include <stdio.h>
#include <limits.h>

int	main(){
	ft_printf(" %\n ", 'x');
	ft_printf(" %c\n ", 'x');
	return 0;
}
 */
