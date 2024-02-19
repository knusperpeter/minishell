/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 15:57:31 by caigner           #+#    #+#             */
/*   Updated: 2024/02/11 19:25:43 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// declare -x is widely used to mark a variable for export to
// environment variables
// $?
#include "../../include/minishell.h"

int	ft_env(t_env *env)
{
	while (env)
	{
		if (!env->flag)
			printf("%s=%s\n", env->variable, env->value);
		env = env->next;
	}
	return (EXIT_SUCCESS);
}
