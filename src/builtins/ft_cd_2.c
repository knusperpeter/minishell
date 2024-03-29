/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 23:07:09 by chris             #+#    #+#             */
/*   Updated: 2024/02/13 23:10:38 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_env_value(t_env *env, char *variable, char *value)
{
	int	i;

	while (env)
	{
		i = ft_strlen(variable);
		if (!ft_strncmp(env->variable, variable, i))
		{
			free(env->value);
			env->value = ft_strdup(value);
			return ;
		}
		env = env->next;
	}
}
