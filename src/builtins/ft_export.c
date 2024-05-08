/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:41 by caigner           #+#    #+#             */
/*   Updated: 2024/05/08 17:13:46 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// declare -x VAR="value" only?
#include "../../include/minishell.h"

void sort_env(t_env *env)
{
	char *temp_var;
	char *temp_val;
	int temp_flag;
	t_env *i;
	
	i = env;
	while (i != NULL)
	{
		t_env *j = i->next;
		while (j != NULL)
		{
			if (ft_strncmp(i->variable, j->variable, sizeof(j->variable) + 1) > 0)
			{
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
			j = j->next;
		}
		i = i->next;
	}
}

void	export_print_env(t_env *env)//add alphabetical order?
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

void	add_to_var(t_env *env, char *arg)
{
	char	*tmp;

	if (env->value)
	{
		tmp = ft_strjoin(env->value, arg);
		//protect
		free(env->value);
		env->value = tmp;
	}
	else
	{
		env->value = ft_strdup(arg);
		//protect
	}
	env->flag = 0;
}

int	already_in_env(char *args, t_env *env, int errorno)
{
	size_t	len;

	len = 0;
	while (args[len] && args[len] != '=')
		len++;
	if (len != 0 && args[len - 1] == '+')
		len--;
	while (env)
	{
		if (!ft_strncmp(args, env->variable, len) && !env->variable[len])
		{
			if (errorno == 0 && args[len] == '=')
			{
				if (env->value)
					free(env->value);
				env->value = ft_strdup(&args[len + 1]);
				if (!env->value)
					exit(EXIT_FAILURE);
				env->flag = 0;
			}
			else if (errorno == 0 && args[len] == '+')
			 	add_to_var(env, &args[len + 2]);
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

static void print_error(t_common *c, char *arg, int i)
{
	if (i == -1 || i == -2)
	{
		c->exitstatus = 1;
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
	}
	else if (i == -3)
	{
		c->exitstatus = 2;
		ft_putstr_fd("minishell: export: ", 2);
		ft_putchar_fd(arg[0], 2);
		ft_putchar_fd(arg[1], 2);		
		ft_putstr_fd(": invalid option\n", 2);
	}
}

//export hallo+=lol oder auch export += $hallo$hallo$hallo
int	ft_export(t_common *c, char **args, t_env *env)
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
		errorno = 0;
		errorno = is_valid_env(args[i]);
		if (errorno < 0)
			print_error(c, args[i], errorno);
		if (!already_in_env(args[i], env, errorno) && errorno >= 0)
			add_variable_to_env(env, args[i], errorno);
		i++;
	}
	return (EXIT_SUCCESS);
}

//Error to still handle: export "" -> 	export: not valid in this context: 