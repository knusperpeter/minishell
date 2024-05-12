/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:24 by caigner           #+#    #+#             */
/*   Updated: 2024/05/12 01:19:10 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//cd /bin goes to /usr/bin
// needs fixing

#include "../../include/minishell.h"

void	join_path_else(char **path, char *oldpwd, char *args)
{
	char	*tmp;

	*path = ft_strdup(oldpwd);
	tmp = *path;
	*path = ft_strjoin(*path, "/");
	free(tmp);
	tmp = *path;
	*path = ft_strjoin(*path, args);
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
		return (get_set_path(c->env, "OLDPWD", path));
	else if (args[1][0] == '/')
		*path = ft_strdup(args[1]);
	else if (!ft_strncmp(args[1], ".", size))
		*path = ft_strdup(*oldpwd);
	else if (!ft_strncmp(args[1], "..", size))
	{
		*path = ft_strdup(*oldpwd);
		tmp = *path;
		*path = ft_strjoin(*path, "/..");
		free(tmp);
	}
	else
		return (join_path_else(path, *oldpwd, args[1]), 0);
	return (0);
}

void	print_error(int errorno, char *arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (arg && errorno == 0)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
		perror(strerror(errno));
		ft_putstr_fd("\n", 2);
	}
	if (errorno == -1)
		ft_putstr_fd("HOME not set\n", 2);
	if (errorno == -2)
		ft_putstr_fd("OLDPWD not set\n", 2);
	if (errorno == -3)
		ft_putstr_fd("too many arguments\n", 2);
}

int	ft_cd(char **args, t_common *c)
{
	char	*path;
	char	*oldpwd;
	int		errorno;

	path = NULL;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (print_error(0, args[1]), EXIT_FAILURE);
	errorno = get_path(args, &oldpwd, &path, c);
	if (chdir(path) == -1 || errorno == -3)
	{
		free(path);
		print_error(errorno, args[1]);
		return (free(oldpwd), EXIT_FAILURE);
	}
	free(path);
	path = getcwd(NULL, 0);
	if (!path)
		return (free(oldpwd), print_error(errorno, args[1]), 1);
	set_env_value(c->env, "OLDPWD", oldpwd);
	set_env_value(c->env, "PWD", path);
	free(oldpwd);
	return (free(path), 0);
}
//	Wenn unset PWD und dann cd, ist auch OLDPWD unsetted.
//	Wie wird PWD neu gesetzt? Wie wird OLDPWD neu gesetzt?