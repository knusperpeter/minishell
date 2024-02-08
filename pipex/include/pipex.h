/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 12:43:49 by caigner           #+#    #+#             */
/*   Updated: 2024/01/30 19:28:15 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../gnl/get_next_line.h"
# include "../libft/libft.h"
# include "../printf/ft_printf.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int		in_fd;
	int		out_fd;
	int		here_doc;
	int		cmd_count;
	int		pipe_nbrs;
	int		*pids;
	int		pidsflag;
	int		*pipes;
	int		pipesflag;
	char	**paths;
	int		pathsflag;
	char	**cmd_paths;
	int		cmd_pathsflag;
	char	***cmd_args;
	int		cmd_argsflag;
}	t_pipex;

int		ft_init_pipex(t_pipex *pipex, int ac, char **av);
int		open_files(t_pipex *pipex, int ac, char **av);
int		ft_parse_args(t_pipex *pipex, char **av);
int		ft_parse_cmds(t_pipex *pipex, char **envp);
int		ft_exec(t_pipex *pipex, int cmd, int pipe);
int		create_pipes(t_pipex *pipex);
void	ft_cleanup(t_pipex *p);
void	free2d(char **arr);

#endif