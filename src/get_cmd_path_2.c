/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 11:30:56 by chris             #+#    #+#             */
/*   Updated: 2024/05/20 15:11:22 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**ft_get_paths(t_common *c, t_env *env)
{
	t_env	*tmp;
	char	**paths;

	tmp = env;
	while (tmp && ft_strncmp(tmp->variable, "PATH", 5))
		tmp = tmp->next;
	if (!tmp)
		return (NULL);
	paths = ft_protect(c, ft_split(tmp->value, ':'), 0, 0);
	return (paths);
}

char	*join_path(t_common *c, char *cmd, char *path)
{
	char	*prepath;
	char	*fullpath;

	if (cmd[0] == '/')
		return (NULL);
	prepath = ft_protect(c, ft_strjoin(path, "/"), 0, 0);
	fullpath = ft_protect(c, ft_strjoin(prepath, cmd), prepath, 0);
	free(prepath);
	return (fullpath);
}

char	*shlvl(t_common *c, char *value)
{
	int		shlvl;
	char	*new_value;

	shlvl = ft_atoi(value) + 1;
	if (shlvl < 0)
		shlvl = 0;
	if (shlvl > 999 && !c->print_shlvl_warning)
	{
		c->print_shlvl_warning = 1;
		ft_putstr_fd("minishell: warning: shell level (", STDERR);
		ft_putstr_fd(value, STDERR);
		ft_putstr_fd(") too high, resetting to 1\n", STDERR);
		shlvl = 1;
	}
	new_value = ft_protect(c, ft_itoa(shlvl), 0, 0);
	return (new_value);
}
