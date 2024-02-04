/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:41 by caigner           #+#    #+#             */
/*   Updated: 2024/02/03 19:18:43 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdlib.h>
#include <string.h>

void	print_env(t_env *env)
{
	while (env)
	{
		printf("declare -x %s=%s\n", env->variable, env->value);
		env = env->next;
	}
}

int	is_valid_env(char *env)
{
	int	i;

	i = 0;
	if (ft_isdigit(env[i]) != 0)
		return (1);
	while (env[i])
	{
		
	}
}

int	ft_export(char **args, t_env *env)
{
	if (!args[1])
	{
		print_env(env);
		return (EXIT_SUCCESS);
	}
	else
	{
		is_valid_env(args[1]);
	}
	return (EXIT_SUCCESS);
}
