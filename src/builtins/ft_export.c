/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:41 by caigner           #+#    #+#             */
/*   Updated: 2024/03/25 22:30:18 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// declare -x VAR="value" only?
#include "../../include/minishell.h"
#include <stdlib.h>

void	export_print_env(t_env *env)//add alphabetical order?
{
	while (env)
	{
		if (!env->flag)
			printf("declare -x %s=\"%s\"\n", env->variable, env->value);
		else
			printf("declare -x %s\n", env->variable);
		env = env->next;
	}
}

int	is_valid_env(char *env)
{
	int	i;

	if (!ft_isalpha(env[0]) && env[0] != '_')
		return (-1);
	i = 0;
	while (env[i] && env[i] != '=')
	{
		if (!ft_isalnum(env[i]) && env[i] != '_')
			return (-2);
		i++;
	}
	if (env[i] == '=')
		return (0);
	return (1);
}

int	already_in_env(char *args, t_env *env, int errorno)
{
	size_t	len;

	len = 0;
	while (args[len] && args[len] != '=')
		len++;
	while (env)
	{
		if (!ft_strncmp(args, env->variable, len) && !env->variable[len])
		{
			if (errorno == 0)
			{
				if (env->value)
					free(env->value);
				env->value = ft_strdup(&args[len + 1]);
				if (!env->value)
					exit(EXIT_FAILURE);
				env->flag = 0;
			}
			return (EXIT_FAILURE);
		}
		env = env->next;
	}
	return (EXIT_SUCCESS);
}

void	add_variable_to_env(t_env *env, char *args, int errorno)
{
	t_env	*new;
	t_env	*prev;

	prev = env;
	while (prev->next)
		prev = prev->next;
	create_list_element((void **) &new, sizeof(t_env));
	if (ft_init_env(new, args, prev) == EXIT_FAILURE)
		perror("malloc failed");
	new->flag = errorno;
	prev->next = new;
}

//export hallo+=lol oder auch export += $hallo$hallo$hallo
int	ft_export(char **args, t_env *env)
{
	int	i;
	int	errorno;

	if (!args)
		return (EXIT_FAILURE);
	if (!args[1])
		return (export_print_env(env), EXIT_SUCCESS);
	i = 1;
	while (args[i])
	{
		errorno = is_valid_env(args[i]);
		if (errorno < 0)
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return (EXIT_FAILURE);
		}
		if (!already_in_env(args[i], env, errorno))
			add_variable_to_env(env, args[i], errorno);
		i++;
	}
	return (EXIT_SUCCESS);
}

//Error to still handle: export "" -> 	export: not valid in this context: 