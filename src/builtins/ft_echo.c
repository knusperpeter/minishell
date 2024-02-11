/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:15 by caigner           #+#    #+#             */
/*   Updated: 2024/02/11 19:26:18 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdlib.h>

int	check_nl(char *arg)
{
	int	i;

	i = 0;
	if (arg && arg[i])
	{
		if (arg[i] == '-' && arg[i + 1] == 'n')
		{
			i++;
			while (arg[i])
			{
				if (arg[i] != 'n' )
					return (EXIT_SUCCESS);
				i++;
			}
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int	ft_echo(char **args)
{
	int	i;
	int	nl;

	i = 1;
	nl = 0;
	while (args[i])
	{
		if (check_nl(args[i]))
			nl++;
		else
		 {
			printf("%s", args[i]);
			if (args[i + 1])
				printf(" ");
		 }
		 i++;
	}
	if (!nl)
		printf("\n");
	return (EXIT_SUCCESS);
}