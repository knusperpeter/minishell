/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:41 by caigner           #+#    #+#             */
/*   Updated: 2024/02/08 13:05:06 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdlib.h>

void	print_env(t_env *env)//add alphabetical order?
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

void	printerror(char *args)
{
	perror("minishell: export: `");
	perror(args);
	perror("': not a valid identifier");
}

int	is_valid_env(char *env)
{
	int	i;

	i = 0;
	if (!ft_isalpha(env[0]) && env[0] != '_')
		return (-1);
	while(env[i] && env[i] != '=')
	{
		if (!ft_isalnum(env[i]) && env[i] != '_')
			return (-2);
		i++;
	}
	if (env[i] == '=')
		return (0);
	return (1);
}

int	already_in_env(char *args, t_env *env)
{
	size_t	len;

	len = 0;
	while (args[len] && args[len] != '=')
		len++;
	while (env)
	{
		if (!ft_strncmp(args, env->variable, len) && (!env->variable[len]))
		{
			if (env->value)
				free(env->value);
			env->value = ft_strdup(&args[len + 1]);
			return (EXIT_FAILURE);
		}
		env = env->next;
	}
	return (EXIT_SUCCESS);
}

int	declare_uninitialized_variable(t_env *node, char *envp, t_env *prev)
{
	int		i;
	int		size;
	
	i = 0;
	size = ft_strlen(envp);
	node->variable = malloc(size + 1);
	if (!node->variable)
		return (EXIT_FAILURE);
	while (i < size)
	{
		node->variable[i] = envp[i];
		i++;
	}
	node->variable[i] = 0;
	node->flag = 1;
	node->value = NULL;
	node->prev = prev;
	node->next = NULL;
	return (EXIT_SUCCESS);
}

void	add_env(t_env *env, char *args, int errorno)
{
	t_env	*new;
	t_env	*prev;

	prev = env;
	while (prev->next)
		prev = prev->next;
	create_list_element((void **) &new, sizeof(t_env));
	if (ft_init_env(new, args, prev) == EXIT_FAILURE)
		declare_uninitialized_variable(new, args, prev);
	new->flag = errorno;
	prev->next = new;
}

int	ft_export(char **args, t_env *env)//export hallo+=lol oder auch export += $hallo$hallo$hallo
{
	int	i;
	int	errorno;

	if (args == NULL)
		return (EXIT_FAILURE);
	i = 1;
	if (!args[1])
	{
		print_env(env);
		return (EXIT_SUCCESS);
	}
	else
	{
		while (args[i])
		{
			errorno = is_valid_env(args[i]);
			if (errorno < 0)
				return (printerror(args[i]), EXIT_FAILURE);
			if (!already_in_env(args[i], env))
				add_env(env, args[i], errorno);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}
