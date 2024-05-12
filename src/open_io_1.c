/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_io_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 13:19:00 by caigner           #+#    #+#             */
/*   Updated: 2024/05/12 22:00:48 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	open_infile(t_common *c, t_io_red *io, t_cmd_table *cmd_node)
{
	int		fd;

	if (io->type == HEREDOC)
	{
		interactive_here(c);
		fd = open(cmd_node->heredoc_name, O_CREAT | O_RDWR
				| O_EXCL, S_IRUSR | S_IWUSR);
		here_doc(c, io, &fd);
		non_interactive(c);
		if (c->exitstatus == 130)
			return (close(fd), 5);
	}
	else
		fd = open(io->infile, O_RDONLY);
	if (fd == -1)
		return (open_failed(io, io->infile), 0);
	if (fd != 0)
		close(fd);
	return (1);
}

int	open_outfile(t_io_red *io, t_cmd_table *cmd_node)
{
	int		fd;

	if (io->type == REDIR_OUT)
		fd = open(io->outfile, O_WRONLY | O_TRUNC
				| O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else
		fd = open(io->outfile, O_WRONLY | O_APPEND
				| O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1 && cmd_node->id != 0)
	{
		if (errno == EISDIR)
			dprintf(2, "minishell: %s: Is a directory\n", io->outfile);
		else
		{
			ft_putstr_fd("minishell: ", 2);
			perror(io->outfile);
		}
		return (open_failed(io, io->outfile), fd);
	}
	if (fd != 1 && cmd_node->id != 0)
		close(fd);
	return (fd);
}

int	open_file(t_common *c, t_io_red *io, t_cmd_table *cmd_node)
{
	if (io->type == HEREDOC || io->type == REDIR_IN)
	{
		cmd_node->in = io;
		return (open_infile(c, io, cmd_node));
	}
	else if (io->type == REDIR_OUT || io->type == APPEND)
	{
		cmd_node->out = io;
		return (open_outfile(io, cmd_node));
	}
	return (1);
}

int	open_io(t_common *c, t_list *io_lst, t_cmd_table *cmd_node)
{
	t_list		*tmp;
	t_io_red	*io;
	int			status;

	status = 1;
	tmp = io_lst;
	while (tmp)
	{
		io = tmp->content;
		ft_close_old_fd(cmd_node, tmp->content);
		status = open_file(c, io, cmd_node);
		if (status == 5)
			return (status);
		if (status == 0)
			unlink_heredoc(io_lst->content, cmd_node);
		if (status == -1)
		{
			c->exitstatus = 1;
		}
		tmp = tmp->next;
	}
	return (status);
}

int	open_redirections(t_common *c, t_cmd_table *cmd_node)
{
	t_io_red	*io;

	(void) c;
	if (cmd_node->in)
	{
		io = cmd_node->in;
		cmd_node->read_fd = open(io->infile, O_RDONLY);
		if (cmd_node->read_fd == -1)
		{
			c->exitstatus = 1;
			return (0);
		}
	}
	if (cmd_node->out)
	{
		io = cmd_node->out;
		cmd_node->write_fd = open_outfile(io, cmd_node);
		if (cmd_node->write_fd == -1)
		{
			c->exitstatus = 1;
			return (0);
		}
	}
	return (1);
}
