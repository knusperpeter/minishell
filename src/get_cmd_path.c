/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:18:28 by caigner           #+#    #+#             */
/*   Updated: 2024/05/12 01:24:07 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * Function: ft_get_paths
 * Description: Retrieves the PATH environment variable and splits it into an
 * array of paths.
 * Parameter: env - The linked list of environment variables.
 * Returns: An array of paths.
 */
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

/**
 * Function: join_path
 * Description: Joins a command and a path into a full path.
 * Parameters: 
 * - cmd: The command to join.
 * - path: The path to join.
 * Returns: The full path.
 */
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

/**
 * Function: add_path
 * Description: Adds the path to the command in the command table.
 * Parameters: 
 * - cmd: The command table.
 * - paths: The array of paths.
 * Returns: EXIT_SUCCESS if successful, EXIT_FAILURE if an error occurred.
 */
int	add_path(t_cmd_table *cmd, char **paths)
{
	char	*path;
	int		i;

	i = 0;
	if (cmd && cmd->str && cmd->str[0])
	{
		if (!access(cmd->str[0], F_OK | X_OK | R_OK) && !is_dir(cmd->str[0]))
		{
			cmd->exec_path = ft_strdup(cmd->str[0]);
			if (!cmd->exec_path)
				ft_printerrno("allocation failed at exec_path");
			return (1);
		}
		else if (cmd->str && cmd->str[0] && (cmd->str[0][0] != '.' && cmd->str[0][1] != '/') && paths)
		{
			while (paths[i])
			{
				path = join_path(cmd->str[0], paths[i++]);
				if (!path)
					return (0);
				if (!access(path, F_OK | X_OK | R_OK) && !is_dir(path))
					return (cmd->exec_path = path, 1);
				free(path);
			}
		}
	}
	return (0);
}

int	is_path_or_pwd(t_cmd_table *cmd, t_env *env)
{
	t_env	*tmp;
	char	*path;

	if (cmd->str[0] && cmd->str[0][0] == '/' && !is_dir(cmd->str[0]))
	{
		cmd->exec_path = ft_strdup(cmd->str[0]);
		return (1);
	}
	tmp = env;
	while (tmp && ft_strncmp(tmp->variable, "PWD", 4))
		tmp = tmp->next;
	path = join_path(cmd->str[0], tmp->value);
	if (!access(path, F_OK | X_OK | R_OK) && !is_dir(path))
		return (cmd->exec_path = path, 1);
	return (free(path), 0);
}

/**
 * Function: get_cmd_path
 * Description: Gets the full path of the command in the command table.
 * Parameters: 
 * - c: The common structure containing the shell data.
 * - cmd: The command table.
 * Returns: EXIT_SUCCESS if successful, EXIT_FAILURE if an error occurred.
 */
int	get_cmd_path(t_common *c, t_cmd_table *cmd)
{
	char	**paths;

	paths = ft_get_paths(c->env);
	if (!paths)
	{
		if (is_path_or_pwd(cmd, c->env))
			return (1);
		return (0);
	}
	if (!add_path(cmd, paths))
	{
		free_2d(paths);
		return (0);
	}
	free_2d(paths);
	return (1);
}
