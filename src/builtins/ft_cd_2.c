/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 23:07:09 by chris             #+#    #+#             */
/*   Updated: 2024/05/18 16:19:33 by caigner          ###   ########.fr       */
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

void	join_relative_path(t_common *c, char **path, char **oldpwd)
{
	char	*tmp;

	*path = ft_protect(c, ft_strdup(*oldpwd), 0, 0);
	tmp = *path;
	*path = ft_protect(c, ft_strjoin(*path, "/.."), tmp, 0);
	free(tmp);
}

int	get_path(char **args, char **oldpwd, char **path, t_common *c)
{
	int		size;
	char	*tmp;

	size = ft_strlen(args[1]);
	if (args[1] && args[2])
		return (-3);
	if (!args[1] || !ft_strncmp(args[1], "~", size))
		return (get_set_path(c->env, "HOME", path));
	else if (!ft_strncmp(args[1], "-", size))
	{
		tmp = get_env_value(c->env, "OLDPWD");
		printf("%s\n", tmp);
		return (get_set_path(c->env, "OLDPWD", path));
	}
	else if (args[1][0] == '/')
		*path = ft_protect(c, ft_strdup(args[1]), 0, 0);
	else if (!ft_strncmp(args[1], ".", size))
		*path = ft_protect(c, ft_strdup(*oldpwd), 0, 0);
	else if (!ft_strncmp(args[1], "..", size))
		join_relative_path(c, path, oldpwd);
	else
		return (join_path_else(c, path, *oldpwd, args[1]), 0);
	return (0);
}
