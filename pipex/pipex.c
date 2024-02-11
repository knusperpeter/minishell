/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 13:48:41 by caigner           #+#    #+#             */
/*   Updated: 2024/02/10 20:31:18 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/pipex.h"
#include <stdlib.h>

int	initialize(int ac, char **av, t_pipex *pipex)
{
	if (ac < 5)
	{
		ft_printf("Error: Not enough arguments\n");
		return (EXIT_FAILURE);
	}
	if (ft_init_pipex(pipex, ac, av) == EXIT_FAILURE)
	{
		ft_cleanup(pipex);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	execute(t_pipex *pipex)
{
	int	i;
	int	pipe;
	int	status;

	i = 0;
	pipe = 0;
	while (pipex->cmd_count > i && pipex->cmd_pathsflag == 0)
	{
		if (pipex->cmd_paths[i] != NULL && pipex->pipesflag == 0)
			status = ft_exec(pipex, i, pipe);
		i++;
		pipe += 2;
	}
	return (status);
}

void	wait_and_clean(t_pipex *pipex)
{
	int	i;

	if (pipex->in_fd != -1)
		close(pipex->in_fd);
	if (pipex->out_fd != -1)
		close(pipex->out_fd);
	i = 0;
	while (i < pipex->pipe_nbrs && pipex->pipesflag == 0)
		close(pipex->pipes[i++]);
	i = pipex->cmd_count - 1;
	while (i >= 0 && pipex->pidsflag == 0)
		waitpid(pipex->pids[i--], NULL, 0);
	ft_cleanup(pipex);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	if (initialize(ac, av, &pipex) == EXIT_FAILURE)
		return (EXIT_SUCCESS);
	open_files(&pipex, ac, av);
	ft_parse_args(&pipex, av);
	ft_parse_cmds(&pipex, envp);
	if (create_pipes(&pipex) == EXIT_SUCCESS)
		execute(&pipex);
	wait_and_clean(&pipex);
	return (EXIT_SUCCESS);
}
