/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:31 by caigner           #+#    #+#             */
/*   Updated: 2024/01/29 18:33:28 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <linux/limits.h>

int	ft_pwd(void)
{
	char	buf[PATH_MAX];

	if (getcwd(buf, sizeof(buf)) == NULL)
		printf("%s\n", buf);
	else
	{
		perror("getcwd() error");
		return (1);
	}
	return (0);
}
