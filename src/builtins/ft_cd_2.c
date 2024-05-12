/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 23:07:09 by chris             #+#    #+#             */
/*   Updated: 2024/05/11 23:55:58 by chris            ###   ########.fr       */
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
			break ;
		}
		env = env->next;
	}
}

static char	*get_env_value(t_env *env, char *variable)
{
	int	i;

	while (env)
	{
		i = ft_strlen(env->variable);
		if (!ft_strncmp(env->variable, variable, i) && !variable[i])
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	get_set_path(t_env *env, char *variable, char **path)
{
	char	*env_value;

	env_value = get_env_value(env, variable);
	if (env_value)
		return (*path = ft_strdup(env_value), 0);
	else if (!ft_strncmp(variable, "HOME", 5))
		return (-1);
	else
		return (-2);
}
