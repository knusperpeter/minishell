/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_io.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 13:19:00 by caigner           #+#    #+#             */
/*   Updated: 2024/03/20 16:21:54 by caigner          ###   ########.fr       */
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
	ft_putstr_fd(strerror(errno), 2);
}

/**
 * Function: here_doc
 * Description: Implements the here document (heredoc) feature of the shell.
 * Parameters: 
 * - limiter: The delimiter string for the heredoc.
 * - cmd_table: The command table structure.
 */
void	here_doc(char *limiter, t_cmd_table *cmd_table)
{
	char		*buf;

	if (cmd_table->read_fd == -1)
		ft_printerrno(limiter);
	while (1)
	{
		write(1, "heredoc> ", 9);
		if (get_next_line(0, &buf, 0) < 0)
			exit (1);
		if (buf == NULL || *buf == '\0')
			break ;
		if (ft_strncmp(limiter, buf, (ft_strlen(buf) - 1)) == 0)
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

/**
 * Function: open_infile
 * Description: Opens the input file for a command.
 * Parameters: 
 * - io: The IO redirection structure.
 * - cmd_node: The command table node.
 * Returns: 0 if successful, EXIT_FAILURE if an error occurred.
 */
int	open_infile(t_io_red *io, t_cmd_table *cmd_node)
{
	int		fd;
	
	if (io->type == HEREDOC)
	{
		fd = open(cmd_node->heredoc_name, O_CREAT | O_WRONLY
				| O_TRUNC, 0644);
		here_doc(io->heredoc_limiter, cmd_node);
	}
	else
		fd = open(io->infile, O_RDONLY);
	if (fd == -1)
	{
		ft_printerrno("filename");
		return (0);
	}
	replace_fd(&cmd_node->read_fd, &fd);
	return (1);
}

int	open_outfile(t_io_red *io, t_cmd_table *cmd_node)
{
	int		fd;
	
	if (io->type == REDIR_OUT)
		fd = open(io->outfile, O_WRONLY | O_TRUNC
				| O_CREAT, S_IRUSR | S_IWUSR);
	else
		fd = open(io->outfile, O_WRONLY | O_APPEND
				| O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		ft_printerrno("filename");
		return (0);
	}
	replace_fd(&cmd_node->write_fd, &fd);
	return (1);
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
int	open_file(t_io_red *io, t_cmd_table *cmd_node)
{
	if (io->type == HEREDOC || io->type == REDIR_IN)
		return (open_infile(io, cmd_node));
	else if (io->type == REDIR_OUT || io->type == APPEND)
		return (open_outfile(io, cmd_node));
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
		if (io->type == HEREDOC || io->type == REDIR_IN)
			safe_close(&cmd_node->read_fd);
		else if (io->type == REDIR_OUT || io->type == APPEND)
			safe_close(&cmd_node->write_fd);	
}
/**
 * Function: open_io
 * Description: Opens the files for IO redirection for a command.
 * Parameters: 
 * - io: The list of IO redirection structures.
 * - cmd_node: The command table node.
 * Returns: EXIT_SUCCESS if successful, EXIT_FAILURE if an error occurred.
 */
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
