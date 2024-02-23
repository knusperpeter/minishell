/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lltoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 08:29:48 by miheider          #+#    #+#             */
/*   Updated: 2024/02/10 20:35:35 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_translate(char *str, long long num, int len_n, int sign)
{
	str[len_n] = '\0';
	while ((len_n) >= 1)
	{
		str[len_n - 1] = (num % 10) + '0';
		num /= 10;
		len_n--;
	}
	if (sign == 1)
		str[0] = '-';
	return (str);
}

static int	length(long long n, int len_n)
{
	while (n > 9)
	{
		n = n / 10;
		len_n++;
	}
	return (len_n);
}

static char	*check_min(char *str)
{
	str = malloc(sizeof(char) * 21);
	if (NULL == str)
		return (0);
	ft_strlcpy(str, "-9223372036854775808", 21);
	return (str);
}

static void	signer(long long int *num, int n, int *sign)
{
	*num = n;
	*sign = 0;
}

char	*ft_lltoa(long long n)
{
	int				sign;
	long long		num;
	int				len_n;
	char			*str;

	signer(&num, n, &sign);
	if (n == -9223372036854775807 -1)
		return (check_min("-9223372036854775808"));
	len_n = 1;
	if (n < 0)
	{
		n *= -1;
		len_n = 2;
		sign = 1;
	}
	num = n;
	len_n = length(num, len_n);
	str = (char *)malloc(sizeof(char) * (len_n + 1));
	if (!str)
		return (NULL);
	str = ft_translate(str, num, len_n, sign);
	return (str);
}
