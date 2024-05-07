/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 15:57:31 by caigner           #+#    #+#             */
/*   Updated: 2024/05/07 16:54:55 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void print_error(char *str)
{
	ft_putstr_fd("env: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

int	ft_env(char **args, t_env *env)
{
	if (args[1])
	{
		print_error(args[1]);
		return (127);
	}
	while (env)
	{
		if (!env->flag)
			printf("%s=%s\n", env->variable, env->value);
		env = env->next;
	}
	return (EXIT_SUCCESS);
}
