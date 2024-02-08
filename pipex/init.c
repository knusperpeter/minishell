/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 14:19:02 by caigner           #+#    #+#             */
/*   Updated: 2024/01/24 15:44:08 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/pipex.h"

int	args_in(char *av, t_pipex *pipex)
{
	if (av && ft_strncmp("here_doc", av, 9) != 0)
	{
		pipex->here_doc = 0;
		return (5);
	}
	else if (av && ft_strncmp("here_doc", av, 9) == 0)
	{
		pipex->here_doc = 1;
		return (6);
	}
	return (EXIT_FAILURE);
}

int	ft_init_pipex(t_pipex *pipex, int ac, char **av)
{
	if (ac < args_in(av[1], pipex))
	{
		ft_printf("Error: Not enough arguments\n");
		return (EXIT_SUCCESS);
	}
	pipex->out_fd = -1;
	pipex->cmd_paths = NULL;
	pipex->paths = NULL;
	pipex->cmd_args = NULL;
	if (!pipex->here_doc)
		pipex->cmd_count = ac - 3;
	else
		pipex->cmd_count = ac - 4;
	pipex->pids = ft_calloc(pipex->cmd_count, sizeof(int));
	if (!pipex->pids)
		return (pipex->pidsflag = 1, EXIT_FAILURE);
	pipex->pipe_nbrs = 2 * (pipex->cmd_count - 1);
	pipex->pipes = ft_calloc(pipex->pipe_nbrs, sizeof(int));
	if (!pipex->pipes)
		return (pipex->pipesflag = 1, EXIT_FAILURE);
	return (pipex->pidsflag = 0, pipex->pipesflag = 0, pipex->cmd_pathsflag = 0,
		EXIT_SUCCESS);
}
