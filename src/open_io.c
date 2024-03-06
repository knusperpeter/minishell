/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_io.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 13:19:00 by caigner           #+#    #+#             */
/*   Updated: 2024/03/06 13:11:23 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_printerrno(char *s)
{
	ft_putstr_fd("minishell: ", 2);
	if (s)
		ft_putstr_fd(s, 2);
	ft_putstr_fd(strerror(errno), 2);
}
//hier kommt es nicht in die history...
void	here_doc(char *limiter, t_cmd_table *cmd_table)
{
	char		*buf;

	if (cmd_table->read_fd == -1)
		ft_printerrno(limiter);
	while (1)
	{
		write(1, "minishell heredoc> ", 15);
		if (get_next_line(0, &buf, 0) < 0)
			exit (1);
		if (buf == NULL || *buf == '\0')
			break ;
		if (ft_strncmp(limiter, buf, ft_strlen(limiter)) == 0)
			break ;
		write(cmd_table->read_fd, buf, ft_strlen(buf));
	}
	get_next_line(0, &buf, 1);
	free(buf);
	close(cmd_table->read_fd);
	cmd_table->read_fd = open(cmd_table->heredoc_name, O_RDONLY);
	if (cmd_table->read_fd == -1)
	{
		ft_printerrno(NULL);
		unlink(cmd_table->heredoc_name);
	}
}

int	open_infile(t_io_red *io, t_cmd_table *cmd_node)
{
	if (io->type == HEREDOC)
		{
			cmd_node->read_fd = open(cmd_node->heredoc_name, O_CREAT | O_WRONLY
					| O_TRUNC, 0644);
			here_doc(io->heredoc_limiter, cmd_node);
		}
	else
		cmd_node->read_fd = open(io->infile, O_RDONLY);
	if (cmd_node->read_fd == -1)
		return (ft_printerrno(io->infile), EXIT_FAILURE);
	return (0);
}

//ACHTUNG: ich kann erst öffnen, wenn expanded wurde!!!
int	open_file(t_io_red *io, t_cmd_table *cmd_node)
{
	if (io->type == HEREDOC || io->type == REDIR_IN)
		return (open_infile(io, cmd_node));
	else if (io->type == REDIR_OUT || io->type == APPEND)
	{
		if (io->type == REDIR_OUT)
			cmd_node->write_fd = open(io->outfile, O_WRONLY | O_TRUNC
					| O_CREAT, S_IRUSR | S_IWUSR);
		else
			cmd_node->write_fd = open(io->outfile, O_WRONLY | O_APPEND
					| O_CREAT, S_IRUSR | S_IWUSR);
		if (cmd_node->write_fd == -1)
			return (ft_printerrno(io->outfile), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	unlink_heredoc(t_io_red *io, t_cmd_table *cmd)//das kann ich tatsächlich einfach in cleanup machen, oder?
{
	if (io->type == HEREDOC)
		unlink(cmd->heredoc_name);
}

void	ft_close_old_fd(t_cmd_table *cmd_node, t_io_red *io)
{
		if ((io->type == HEREDOC || io->type == REDIR_IN) && cmd_node->read_fd != 0)
			safe_close(&cmd_node->read_fd);
		else if ((io->type == REDIR_OUT || io->type == APPEND)
				&& cmd_node->write_fd != 1)
			safe_close(&cmd_node->write_fd);	
}

int	open_io(t_list *io, t_cmd_table *cmd_node)
{
	t_list		*tmp;
	int			status;

	status = EXIT_SUCCESS;
	tmp = io;
	while (tmp)
	{
		ft_close_old_fd(cmd_node, tmp->content);
		if (open_file(tmp->content, cmd_node) == EXIT_FAILURE)
		{
			unlink_heredoc(io->content, cmd_node);
			status = EXIT_FAILURE;
		}
//if multiple infiles -> just take the last one. This should happen already in open_file
		tmp = tmp->next;
	}
	return (status);
}

/* Restoration function, if needed:
void	open_file(t_io_red *io, t_cmd_table *cmd_node)
{
	if (io->type == HEREDOC)
	{
		cmd_node->read_fd = open(cmd_node->heredoc_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		here_doc(io->heredoc_limiter, cmd_node);
		
	}
	else if (io->type == REDIR_IN)
	{
		cmd_node->read_fd = open(io->infile, O_RDONLY);

	}
	else if (io->type == REDIR_OUT)
	{
		cmd_node->write_fd = open(io->outfile, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR
				| S_IWUSR);
	}
	else if (io->type == APPEND)
	{
		cmd_node->write_fd = open(io->outfile, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR
				| S_IWUSR);
	}
	if (cmd_node->write_fd == -1)
		printf("minishell: %s: %s\n", io->outfile, strerror(errno));
} */