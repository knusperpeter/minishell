/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:24 by caigner           #+#    #+#             */
/*   Updated: 2024/05/19 12:54:29 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//cd /bin goes to /usr/bin
// needs fixing

#include "../../include/minishell.h"

void	join_path_else(t_common *c, char **path, char *oldpwd, char *args)
{
	char	*tmp;

	tmp = ft_protect(c, ft_strdup(oldpwd), 0, 0);
	*path = ft_protect(c, ft_strjoin(tmp, "/"), tmp, 0);
	free(tmp);
	tmp = *path;
	*path = ft_protect(c, ft_strjoin(tmp, args), tmp, 0);
	free(tmp);
}

void	print_error(int errorno, char *arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg, 2);
	if (errorno == -1)
		ft_putstr_fd("HOME not set", 2);
	if (errorno == -2)
		ft_putstr_fd("OLDPWD not set", 2);
	if (errorno == -3)
		ft_putstr_fd("too many arguments", 2);
	if (errno != 0)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
	}
	ft_putstr_fd("\n", 2);
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
	if (!path)
		return (free(oldpwd), print_error(errorno, ""), EXIT_FAILURE);
	if (chdir(path) == -1 || errorno == -3)
	{
		print_error(errorno, args[1]);
		return (free(path), free(oldpwd), EXIT_FAILURE);
	}
	free(path);
	path = getcwd(NULL, 0);
	if (!path)
		return (free(oldpwd), print_error(errorno, args[1]), 1);
	set_env_value(c->env, "OLDPWD", oldpwd);
	set_env_value(c->env, "PWD", path);
	set_env_value(c->hidden_env, "PWD", path);
	free(oldpwd);
	return (free(path), 0);
}
//	Wenn unset PWD und dann cd, ist auch OLDPWD unsetted.
//	Wie wird PWD neu gesetzt? Wie wird OLDPWD neu gesetzt?