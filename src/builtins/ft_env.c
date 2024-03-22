/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 15:57:31 by caigner           #+#    #+#             */
/*   Updated: 2024/03/22 01:49:07 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
