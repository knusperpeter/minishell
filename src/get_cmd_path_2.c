/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 11:30:56 by chris             #+#    #+#             */
/*   Updated: 2024/05/12 11:30:57 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**ft_get_paths(t_env *env)
{
	t_env	*tmp;
	char	**paths;

	tmp = env;
	while (tmp && ft_strncmp(tmp->variable, "PATH", 5))
		tmp = tmp->next;
	if (!tmp)
		return (NULL);
	paths = ft_split(tmp->value, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

char	*join_path(char *cmd, char *path)
{
	char	*prepath;
	char	*fullpath;

	if (cmd[0] == '/')
		return (NULL);
	prepath = ft_strjoin(path, "/");
	if (!prepath)
		return (NULL);
	fullpath = ft_strjoin(prepath, cmd);
	free(prepath);
	if (!fullpath)
		return (NULL);
	return (fullpath);
}
