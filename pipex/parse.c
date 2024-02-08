/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 14:51:33 by caigner           #+#    #+#             */
/*   Updated: 2024/01/30 19:27:09 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/pipex.h"

int	find_paths(t_pipex *pipex, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH", 4) != 0)
		i++;
	if (!envp[i])
		return (2);
	pipex->paths = ft_split(envp[i] + 5, ':');
	if (!pipex->paths)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

char	*create_path(t_pipex *pipex, int i, int h)
{
	char	*path;
	char	*prepath;

	if (pipex->cmd_args[h][0][0] == '/')
		return (NULL);
	prepath = ft_strjoin(pipex->paths[i], "/");
	if (!prepath)
	{
		prepath = NULL;
		return (NULL);
	}
	path = ft_strjoin(prepath, pipex->cmd_args[h][0]);
	if (!path)
	{
		path = NULL;
		return (free(prepath), NULL);
	}
	free(prepath);
	prepath = NULL;
	return (path);
}

int	add_path(t_pipex *pipex, int h, int *status)
{
	int		i;
	char	*path;

	i = 0;
	while (pipex->paths[i] && pipex->cmd_args[h] && *status == -1)
	{
		if (access(pipex->cmd_args[h][0], F_OK | X_OK | R_OK) == -1)
			path = create_path(pipex, i++, h);
		else
			path = ft_strdup(pipex->cmd_args[h][0]);
		if (!path)
			return (EXIT_FAILURE);
		*status = access(path, F_OK | X_OK | R_OK);
		if (*status != -1)
		{
			pipex->cmd_paths[h] = ft_strdup(path);
			if (!pipex->cmd_paths[h])
			{
				free(path);
				return (EXIT_FAILURE);
			}
		}
		free(path);
	}
	return (EXIT_SUCCESS);
}

int	file_path(t_pipex *pipex, int h, int count)
{
	int		status;

	while (h < count && pipex->cmd_argsflag == 0)
	{
		status = -1;
		if (pipex->paths && pipex->cmd_argsflag == 0)
		{
			add_path(pipex, h, &status);
			if (!pipex->cmd_args[h])
				status = -2;
		}
		if ((status == -1 && pipex->cmd_args[h]))
		{
			if (ft_strchr(pipex->cmd_args[h][0], '/')
				|| access(pipex->cmd_args[h][0], F_OK | X_OK | R_OK) == -1)
				ft_printf("pipex: %s: %s\n", pipex->cmd_args[h][0],
					strerror(errno));
		}
		else if (status == -1 && pipex->cmd_args[h])
			ft_printf("%s: command not found\n", pipex->cmd_args[h][0]);
		else if (!pipex->cmd_args[h] && status == -2)
			ft_printf("pipex: '': No such file or directory\n");
		h++;
	}
	return (EXIT_SUCCESS);
}

int	ft_parse_cmds(t_pipex *pipex, char **envp)
{
	int		h;
	int		count;

	pipex->cmd_paths = ft_calloc((pipex->cmd_count), sizeof(char *));
	if (!pipex->cmd_paths)
		return (pipex->cmd_pathsflag = 1, EXIT_FAILURE);
	pipex->cmd_pathsflag = 0;
	if (find_paths(pipex, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	h = 0;
	count = pipex->cmd_count;
	if (pipex->in_fd == -1)
		h++;
	if (pipex->out_fd == -1)
		count--;
	if (file_path(pipex, h, count) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (0);
}
