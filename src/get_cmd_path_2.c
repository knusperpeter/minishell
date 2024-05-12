/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 11:30:56 by chris             #+#    #+#             */
/*   Updated: 2024/05/12 23:50:13 by caigner          ###   ########.fr       */
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
	paths = ft_protect(c, ft_split(tmp->value, ':'), 0, 0, 0);
	return (paths);
}

char	*join_path(t_common *c, char *cmd, char *path)
{
	char	*prepath;
	char	*fullpath;

	if (cmd[0] == '/')
		return (NULL);
	prepath = ft_protect(c, ft_strjoin(path, "/"), 0, 0, 0);
	fullpath = ft_protect(c, ft_strjoin(prepath, cmd), prepath, 0, 0);
	free(prepath);
	return (fullpath);
}
