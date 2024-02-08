/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 11:55:26 by caigner           #+#    #+#             */
/*   Updated: 2023/09/08 13:27:54 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	const char	*p;
	int			i;

	i = 0;
	while (s[i])
		i++;
	p = s + i;
	while (p >= s)
	{
		if ((char)c == *p)
			return ((char *)p);
		p--;
	}
	return (0);
}
