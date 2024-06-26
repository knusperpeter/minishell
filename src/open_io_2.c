/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_io_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 11:02:35 by chris             #+#    #+#             */
/*   Updated: 2024/05/18 21:44:46 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	open_failed(t_common *c, t_io_red *io, char *file, t_cmd_table *cmd)
{
	if (errno == EISDIR)
		dir_error(c, io->outfile);
	else if (io->type != HEREDOC)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		cmd->permission = 0;
	}
}

void	unlink_heredoc(t_io_red *io, t_cmd_table *cmd)
{
	if (io->type == HEREDOC)
		unlink(cmd->hd_name);
}

void	ft_close_old_fd(t_cmd_table *cmd_node, t_io_red *io)
{
	if ((io->type == HEREDOC || io->type == REDIR_IN)
		&& cmd_node->read_fd != 0)
		close(cmd_node->read_fd);
	else if ((io->type == REDIR_OUT || io->type == APPEND)
		&& cmd_node->write_fd != 1)
		close(cmd_node->write_fd);
}

int	here_doc(t_common *c, t_io_red *io, int *fd)
{
	char		*buf;

	while (g_signal == 0)
	{
		write(1, "> ", 2);
		if (get_next_line_heredoc(0, &buf, 0) < 0)
			ft_cleanup_loop(c);
		if (buf == NULL || *buf == '\0')
			return (write(1, "\n", 1));
		if (ft_strlen(io->heredoc_limiter) == ft_strlen(buf) - 1 && \
				!ft_strncmp(io->heredoc_limiter, buf, (ft_strlen(buf) - 1)))
			break ;
		heredoc_expansion(c, io, &buf);
		write(*(fd), buf, ft_strlen(buf));
		free(buf);
		buf = NULL;
	}
	get_next_line_heredoc(0, &buf, 1);
	if (buf)
		free(buf);
	close(*(fd));
	*(fd) = open(io->infile, O_RDONLY);
	if (*fd == -1)
		unlink(io->infile);
	return (0);
}

void	dir_error(t_common *c, char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	c->exitstatus = 126;
}
