/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_io.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 13:19:00 by caigner           #+#    #+#             */
/*   Updated: 2024/05/11 23:56:02 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * Function: ft_printerrno
 * Description: Prints the error message associated with the current errno value.
 * Parameter: s - An optional string to print before the error message.
 */
void	ft_printerrno(char *s)
{
	ft_putstr_fd("minishell: ", 2);
	if (s)
		ft_putstr_fd(s, 2);
	ft_putchar_fd('\n', 2);
//	ft_putstr_fd(strerror(errno), 2);
}

/**
 * Function: here_doc
 * Description: Implements the here document (heredoc) feature of the shell.
 * Parameters: 
 * - limiter: The delimiter string for the heredoc.
 * - cmd_table: The command table structure.
 */
void	here_doc(t_common *c, t_io_red *io, t_cmd_table *cmd_table, int *fd)
{
	char		*buf;

	//if (*(fd) == -1)
	//	ft_clean_exit(c, "heredoc_fail", 1);//protect...
	(void) cmd_table;
	while (g_signal == 0)
	{
		write(1, "> ", 2);
		if (get_next_line_heredoc(0, &buf, 0) < 0)
			ft_cleanup_loop(c);
		if (buf == NULL || *buf == '\0')
			break ;
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
	{
		unlink(io->infile);
	}
}

/**
 * Function: open_infile
 * Description: Opens the input file for a command.
 * Parameters: 
 * - io: The IO redirection structure.
 * - cmd_node: The command table node.
 * Returns: 0 if successful, EXIT_FAILURE if an error occurred.
 */
int	open_infile(t_common *c, t_io_red *io, t_cmd_table *cmd_node)
{
	int		fd;
	
	if (io->type == HEREDOC)
	{
		interactive_here(c);
		fd = open(cmd_node->heredoc_name, O_CREAT | O_RDWR
				| O_EXCL, S_IRUSR | S_IWUSR);
		here_doc(c, io, cmd_node, &fd);
		non_interactive(c);
		if (c->exitstatus == 130)
		{
			ft_cleanup_loop(c);	
			exit (c->exitstatus);
		}
	}
	else
		fd = open(io->infile, O_RDONLY);
	if (fd == -1)
	{
		if (errno == EISDIR)
			dprintf(2, "minishell: %s: Is a directory\n", io->infile);
		else if (io->type != HEREDOC)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(io->infile);
		}
		return (0);
	}
	if (cmd_node->read_fd != 0)
		close(cmd_node->read_fd);
	cmd_node->read_fd = fd;
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
		return (fd);
	}
	if (cmd_node->write_fd != 1)
		close(cmd_node->write_fd);
	cmd_node->write_fd = fd;
	return (fd);
}

/**
 * Function: open_file
 * Description: Opens the file for IO redirection.
 * Parameters: 
 * - io: The IO redirection structure.
 * - cmd_node: The command table node.
 * Returns: EXIT_SUCCESS if successful, EXIT_FAILURE if an error occurred.
 */

//ACHTUNG: ich kann erst öffnen, wenn expanded wurde!!!
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
/**
 * Function: unlink_heredoc
 * Description: Deletes the temporary file used for a heredoc.
 * Parameters: 
 * - io: The IO redirection structure.
 * - cmd: The command table structure.
 */
void	unlink_heredoc(t_io_red *io, t_cmd_table *cmd)//das kann ich tatsächlich einfach in cleanup machen, oder?
{
	if (io->type == HEREDOC)
		unlink(cmd->heredoc_name);
}
/**
 * Function: ft_close_old_fd
 * Description: Closes the old file descriptors before opening new ones.
 * Parameters: 
 * - cmd_node: The command table node.
 * - io: The IO redirection structure.
 */
void	ft_close_old_fd(t_cmd_table *cmd_node, t_io_red *io)
{
		if ((io->type == HEREDOC || io->type == REDIR_IN) && cmd_node->read_fd != 0)
			close(cmd_node->read_fd);
		else if ((io->type == REDIR_OUT || io->type == APPEND) && cmd_node->write_fd != 1)
			close(cmd_node->write_fd);	
}
/**
 * Function: open_io
 * Description: Opens the files for IO redirection for a command.
 * Parameters: 
 * - io: The list of IO redirection structures.
 * - cmd_node: The command table node.
 * Returns: EXIT_SUCCESS if successful, EXIT_FAILURE if an error occurred.
 */
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
		if (status == 0)
			unlink_heredoc(io_lst->content, cmd_node);
		if (status == -1)
		{
			c->exitstatus = 1;
		}
//if multiple infiles -> just take the last one. This should happen already in open_file
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
