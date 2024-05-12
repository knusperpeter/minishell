/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 10:41:34 by chris             #+#    #+#             */
/*   Updated: 2024/05/13 01:29:45 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	help_sort(t_env *i, t_env *j)
{
	char	*temp_var;
	char	*temp_val;
	int		temp_flag;

	temp_var = i->variable;
	temp_val = i->value;
	temp_flag = i->flag;
	i->variable = j->variable;
	i->value = j->value;
	i->flag = j->flag;
	j->variable = temp_var;
	j->value = temp_val;
	j->flag = temp_flag;
}

void	sort_env(t_env *env)
{
	t_env	*i;
	t_env	*j;
	int		size;

	i = env;
	while (i != NULL)
	{
		j = i->next;
		while (j != NULL)
		{
			size = sizeof(j->variable) + 1;
			if (ft_strncmp(i->variable, j->variable, size) > 0)
				help_sort(i, j);
			j = j->next;
		}
		i = i->next;
	}
}

void	export_print_env(t_env *env)
{
	sort_env(env);
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

	if (env[0] == '-')
		return (-3);
	if (!ft_isalpha(env[0]) && env[0] != '_')
		return (-1);
	i = 0;
	while (env[i] && env[i] != '=')
	{
		if (!ft_isalnum(env[i]) && env[i] != '_')
		{
			if (env[i] == '+' && env[i + 1] == '=')
				return (0);
			else
				return (-2);
		}
		i++;
	}
	if (env[i] == '=')
		return (0);
	return (1);
}

void	add_to_var(t_common *c, t_env *env, char *arg)
{
	char	*tmp;

	if (env->value)
	{
		tmp = ft_protect(c, ft_strjoin(env->value, arg), env->value, 0);
		free(env->value);
		env->value = tmp;
	}
	else
		env->value = ft_protect(c, ft_strdup(arg), 0, 0));
	env->flag = 0;
}
