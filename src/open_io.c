/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_io.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 13:19:00 by caigner           #+#    #+#             */
/*   Updated: 2024/02/24 18:28:30 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	here_doc(char *limiter, t_cmd_table *cmd_table)
{
	char		*buf;

	if (cmd_table->read_fd == -1)
		printf("pipex: %s: %s\n", limiter, strerror(errno));
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
		unlink(cmd_table->heredoc_name);
		printf("minishell: %s\n", strerror(errno));
	}
}

//add return value?
//ACHTUNG: ich kann erst öffnen, wenn expanded wurde!!!
//erst expanden, dann mit cmd_table durchiterieren

void	open_file(t_io_red *io, t_cmd_table *cmd_node)
{
	if (io->type == HEREDOC || io->type == REDIR_IN)
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
			printf("minishell: %s: %s\n", io->infile, strerror(errno));
	}
	else if (io->type == REDIR_OUT || io->type == APPEND)
	{
		if (io->type == REDIR_OUT)
			cmd_node->write_fd = open(io->outfile, O_WRONLY | O_TRUNC
					| O_CREAT, S_IRUSR | S_IWUSR);
		else
			cmd_node->write_fd = open(io->outfile, O_WRONLY | O_APPEND
					| O_CREAT, S_IRUSR | S_IWUSR);
		if (cmd_node->write_fd == -1)
			printf("minishell: %s: %s\n", io->outfile, strerror(errno));
	}
}

void	open_io(t_list *io, t_cmd_table *cmd_node)
{
	t_list		*tmp;
	t_io_red	*io_red;

	tmp = io;
	while (tmp)
	{
		io_red = tmp->content;
		open_file(tmp->content, cmd_node);

		
		tmp = tmp->next;
	}
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