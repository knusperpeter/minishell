/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 15:19:09 by caigner           #+#    #+#             */
/*   Updated: 2024/01/24 15:52:14 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/pipex.h"

int	close_fds(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->pipe_nbrs)
	{
		close(pipex->pipes[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	create_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->pipe_nbrs)
	{
		if (pipe(&pipex->pipes[i]) == -1)
		{
			ft_printf("pipex: %s\n", strerror(errno));
			return (EXIT_FAILURE);
		}
		i += 2;
	}
	return (EXIT_SUCCESS);
}

void	preexec(int cmd, int pipe, t_pipex *pipex)
{
	if (cmd == 0 && pipex->in_fd != -1)
	{
		if (dup2(pipex->in_fd, 0) == -1)
			ft_printf("pipex: %s\n", strerror(errno));
	}
	else if (cmd != 0)
	{
		if (dup2(pipex->pipes[pipe - 2], 0) == -1)
			ft_printf("pipex: %s\n", strerror(errno));
	}
	if (cmd != (pipex->cmd_count - 1))
	{
		if (dup2(pipex->pipes[pipe + 1], 1) == -1)
			ft_printf("pipex: %s\n", strerror(errno));
	}
	else if (cmd == (pipex->cmd_count - 1) && pipex->out_fd != -1)
	{
		if (dup2(pipex->out_fd, 1) == -1)
			ft_printf("pipex: %s\n", strerror(errno));
	}
	if (pipex->in_fd != -1)
		close(pipex->in_fd);
	if (pipex->out_fd != -1)
		close(pipex->out_fd);
	close_fds(pipex);
}

int	ft_exec(t_pipex *pipex, int cmd, int pipe)
{
	pipex->pids[cmd] = fork();
	if (pipex->pids[cmd] == -1)
		return (EXIT_FAILURE);
	if (pipex->pids[cmd] == 0)
	{
		preexec(cmd, pipe, pipex);
		execve(pipex->cmd_paths[cmd], pipex->cmd_args[cmd], NULL);
		exit (EXIT_FAILURE);
	}
	if (cmd != pipex->cmd_count - 1)
		close(pipex->pipes[pipe + 1]);
	else if (cmd == pipex->cmd_count - 1)
		close(pipex->out_fd);
	return (EXIT_SUCCESS);
}
