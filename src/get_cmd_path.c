/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:18:28 by caigner           #+#    #+#             */
/*   Updated: 2024/03/08 14:37:09 by caigner          ###   ########.fr       */
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

int	is_dir(char *file)
{
	struct stat	s;
	
	if (stat(file, &s) == 0)
	{
		if (S_ISDIR(s.st_mode))
			return (1);
	}
	return (0);
}

int	add_path(t_cmd_table *cmd, char **paths)
{
	char	*path;
	int		i;
	
	i = 0;
	if (cmd && cmd->str && cmd->str[0])
	{
		if (access(cmd->str[0], F_OK | X_OK | R_OK) == 0)
			return (cmd->exec_path = cmd->str[0], EXIT_SUCCESS);
		else if (cmd->str && cmd->str[0] && paths)
		{
			while (paths[i])
			{
				path = join_path(cmd->str[0], paths[i++]);
				if (!path)
					return (EXIT_FAILURE);
				if (!access(path, F_OK | X_OK | R_OK) && !is_dir(path))
					return (cmd->exec_path = path, EXIT_SUCCESS);
				free(path);
			}
		}
	}
	return (EXIT_FAILURE);
}

int	get_cmd_path(t_common *c, t_cmd_table *cmd)
{
	char	**paths;

	paths = ft_get_paths(c->env);
	if (!paths)
		return (EXIT_FAILURE);
	add_path(cmd, paths);
	free_2d(paths);
	return (EXIT_SUCCESS);
}
