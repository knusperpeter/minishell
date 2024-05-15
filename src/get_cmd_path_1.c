/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:18:28 by caigner           #+#    #+#             */
/*   Updated: 2024/05/14 16:49:02 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * Function: is_dir
 * Description: Checks if a file is a directory.
 * Parameter: file - The file to check.
 * Returns: 1 if the file is a directory, 0 otherwise.
 */
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

void	access_denied(t_common *c, t_cmd_table *cmd)
{
	cmd->exec_path = ft_protect(c, ft_strdup(cmd->str[0]), 0, 0);
}

int	add_path(t_common *c, t_cmd_table *cmd, char **paths)
{
	char	*path;
	int		i;

	i = 0;
	if (cmd && cmd->str && cmd->str[0])
	{
		if (!access(cmd->str[0], F_OK | X_OK | R_OK) && !is_dir(cmd->str[0]))
			return (access_denied(c, cmd), 1);
		else if (cmd->str && cmd->str[0]
			&& (ft_strncmp(cmd->str[0], "./", 2)) && paths)
		{
			while (paths[i])
			{
				path = join_path(c, cmd->str[0], paths[i++]);
				if (!access(path, F_OK | X_OK | R_OK) && !is_dir(path))
					return (cmd->exec_path = path, 1);
				free(path);
			}
		}
	}
	return (0);
}

int	is_path_or_pwd(t_common *c, t_cmd_table *cmd, t_env *env)
{
	t_env	*tmp;
	char	*path;

	if (cmd->str[0] && cmd->str[0][0] == '/' && !is_dir(cmd->str[0]))
	{
		cmd->exec_path = ft_protect(c, ft_strdup(cmd->str[0]), 0, 0);
		return (1);
	}
	tmp = env;
	while (tmp && ft_strncmp(tmp->variable, "PWD", 4))
		tmp = tmp->next;
	path = join_path(c, cmd->str[0], tmp->value);
	if (!access(path, F_OK | X_OK | R_OK) && !is_dir(path))
		return (cmd->exec_path = path, 1);
	return (free(path), 0);
}

int	get_cmd_path(t_common *c, t_cmd_table *cmd)
{
	char	**paths;

	paths = ft_get_paths(c, c->env);
	if (!paths)
	{
		if (is_path_or_pwd(c, cmd, c->env))
			return (1);
		return (0);
	}
	if (!add_path(c, cmd, paths))
	{
		free_2d(paths);
		return (0);
	}
	free_2d(paths);
	return (1);
}
