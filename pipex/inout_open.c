/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inout_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 14:30:10 by caigner           #+#    #+#             */
/*   Updated: 2024/01/29 16:35:20 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/pipex.h"
#include <sys/types.h>

void	here_doc(char *limiter, t_pipex *pipex)
{
	char	*buf;

	if (pipex->in_fd == -1)
		ft_printf("pipex: %s: %s\n", limiter, strerror(errno));
	while (1)
	{
		write(1, "pipex heredoc> ", 15);
		if (get_next_line(0, &buf, 0) < 0)
			exit (1);
		if (buf == NULL || *buf == '\0')
			break ;
		if (ft_strncmp(limiter, buf, ft_strlen(limiter)) == 0)
			break ;
		write(pipex->in_fd, buf, ft_strlen(buf));
		free(buf);
	}
	get_next_line(0, &buf, 1);
	free(buf);
	close(pipex->in_fd);
	pipex->in_fd = open(".heredoc_tmp", O_RDONLY);
	if (pipex->in_fd == -1)
	{
		unlink(".heredoc_tmp");
		ft_printf("pipex: %s\n", strerror(errno));
	}
}

int	open_files(t_pipex *pipex, int ac, char **av)
{
	if (pipex->here_doc)
	{
		pipex->in_fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		here_doc(av[2], pipex);
	}
	else
	{
		pipex->in_fd = open(av[1], O_RDONLY);
		if (pipex->in_fd == -1)
			ft_printf("pipex: %s: %s\n", av[1], strerror(errno));
	}
	if (pipex->here_doc)
		pipex->out_fd = open(av[ac - 1], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR
				| S_IWUSR);
	else
		pipex->out_fd = open(av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR
				| S_IWUSR);
	if (pipex->out_fd == -1)
		ft_printf("pipex: %s: %s\n", av[ac - 1], strerror(errno));
	return (EXIT_SUCCESS);
}

void	ft_args(char **av, t_pipex *pipex, int i)
{
	char	**tmp;

	tmp = ft_split(av[2 + i + pipex->here_doc], ' ');
	if (!tmp)
	{
		if (ft_strchr(av[2 + i + pipex->here_doc], ' '))
		{
			pipex->cmd_args[i] = ft_calloc(2, sizeof(char *));
			if (!pipex->cmd_args[i])
				return ;
			*pipex->cmd_args[i] = ft_strdup(av[2 + i + pipex->here_doc]);
			if (!*pipex->cmd_args[i])
				return ;
		}
		else
			pipex->cmd_args[i] = NULL;
	}
	else if (tmp)
		pipex->cmd_args[i] = tmp;
}

int	ft_parse_args(t_pipex *pipex, char **av)
{
	int	i;

	i = 0;
	pipex->cmd_args = ft_calloc((pipex->cmd_count), sizeof(char *));
	if (!pipex->cmd_args)
		return (pipex->cmd_argsflag = 1, EXIT_FAILURE);
	while (i < pipex->cmd_count)
	{
		ft_args(av, pipex, i);
		i++;
	}
	return (pipex->cmd_argsflag = 0, EXIT_SUCCESS);
}
