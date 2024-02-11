/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:45 by caigner           #+#    #+#             */
/*   Updated: 2024/02/11 18:58:28 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_env_node(t_env *env)
{
	if (env->value)
		free(env->value);
	if (env->variable)
		free(env->variable);
	free(env);
}

void	remove_node(t_env *env)
{
	t_env	*prev;
	t_env	*next;

	prev = env->prev;
	next = env->next;
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	free_env_node(env);
}

int	check_and_unset(char *arg, t_env *env)
{
	size_t	len;

	len = 0;
	while (arg[len])
		len++;
	while (env)
	{
		if (!ft_strncmp(arg, env->variable, len) && !env->variable[len])
		{
			remove_node(env);
			return (EXIT_SUCCESS);
		}
		env = env->next;
	}
	return (EXIT_FAILURE);
}

int	ft_unset(char **args, t_common *c)
{
	int	i;

	i = 1;
	while (args[i])
	{
		check_and_unset(args[i], c->env);
		i++;
	}
	return (EXIT_SUCCESS);
}
