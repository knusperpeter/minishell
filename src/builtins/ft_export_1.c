/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:41 by caigner           #+#    #+#             */
/*   Updated: 2024/05/12 10:47:05 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// declare -x VAR="value" only?
#include "../../include/minishell.h"

void	replace_env_value(t_common *c, t_env *env, char *args, size_t len)
{
	if (env->value)
		free(env->value);
	env->value = ft_strdup(&args[len + 1]);
	if (!env->value)
		ft_clean_exit(c, "malloc-error", 1);
	env->flag = 0;
}

int	already_in_env(t_common *c, char *args, t_env *env, int errorno)
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
				replace_env_value(c, env, args, len);
			else if (errorno == 0 && args[len] == '+')
				add_to_var(c, env, &args[len + 2]);
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

static void	print_error(t_common *c, char *arg, int i)
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
		if (!already_in_env(c, args[i], env, errorno) && errorno >= 0)
			add_variable_to_env(env, args[i], errorno);
		i++;
	}
	return (EXIT_SUCCESS);
}

//Error to still handle: export "" -> 	export: not valid in this context: 