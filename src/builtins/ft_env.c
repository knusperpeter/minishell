/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 15:57:31 by caigner           #+#    #+#             */
/*   Updated: 2024/01/31 22:29:43 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//declare -x is widely used to mark a variable for export to environment variables
#include "../../include/minishell.h"

int	ft_env(t_env *env)
{
	int	i;

	i = 0;
	while (env && env->next)
	{
		printf("%s\n", env->variable);
		env = env->next;
	}
	if (env)
		printf("%s\n", env->variable);
	return (0);
}