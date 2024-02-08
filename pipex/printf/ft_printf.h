/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 12:29:20 by caigner           #+#    #+#             */
/*   Updated: 2023/11/13 19:39:59 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../libft/libft.h"
# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>

int		ft_printf(const char *format, ...);
int		ft_putchar(const char c);
int		ft_putnbr(long long n);
int		ft_putstr(const char *s);
int		ft_puthexnbr(unsigned int nbr, const char capital);
int		ft_putptr(unsigned long int nbr);
int		ft_unsignednbr(unsigned int n);
int		ft_putnbrptr(unsigned long int nbr);

#endif