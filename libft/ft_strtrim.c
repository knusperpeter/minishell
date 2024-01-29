/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 20:14:21 by miheider          #+#    #+#             */
/*   Updated: 2023/09/15 10:53:42 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*t1;
	int		t1_len;
	int		i;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	t1 = (char *)s1;
	t1_len = ft_strlen(t1);
	i = 0;
	while (i <= (t1_len - 1) && ft_strchr(set, t1[i]))
		i++;
	if (i >= t1_len)
		return (ft_strdup(""));
	while ((t1_len - 1) > 0 && (ft_strchr(set, t1[t1_len - 1])))
		t1_len--;
	str = malloc(sizeof(char) * ((t1_len - 1) - i + 2));
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, &s1[i], (t1_len - 1) - i + 2);
	return (str);
}

/*
#include <stdio.h>
int main(void)
{
    char s1[] = "";
    char set[] = "";

    printf("%s", ft_strtrim(s1, set));
    return 0;
}*/
