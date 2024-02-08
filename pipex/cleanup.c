/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:41:14 by caigner           #+#    #+#             */
/*   Updated: 2024/01/24 15:41:42 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/pipex.h"

void	free_cmd_paths(t_pipex *p, int i)
{
	if (p->cmd_paths)
	{
		while (i < p->cmd_count)
		{
			if (p->cmd_paths[i])
			{
				free(p->cmd_paths[i]);
				p->cmd_paths[i] = NULL;
			}
			i++;
		}
		free(p->cmd_paths);
	}
	if (p->here_doc)
		unlink(".heredoc_tmp");
}

void	ft_cleanup(t_pipex *p)
{
	int	i;

	i = 0;
	if (p != NULL)
	{
		free_cmd_paths(p, i);
		if (p->paths)
			free2d(p->paths);
		i = 0;
		if (p->cmd_args)
		{
			while (i < p->cmd_count)
			{
				if (p->cmd_args[i])
					free2d(p->cmd_args[i]);
				i++;
			}
			free(p->cmd_args);
		}
		if (p->pids && p->pidsflag == 0)
			free(p->pids);
		if (p->pipes && p->pipesflag == 0)
			free(p->pipes);
	}
}

void	free2d(char **arr)
{
	int	i;

	i = 0;
	if (arr != NULL)
	{
		while (arr[i])
		{
			free(arr[i]);
			arr[i] = NULL;
			i++;
		}
		free(arr);
		arr = NULL;
	}
}
