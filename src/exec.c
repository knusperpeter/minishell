/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:25:50 by chris             #+#    #+#             */
/*   Updated: 2024/02/29 14:16:32 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
/*
if i only have one cmd, do i still need to fork? 
	->probably yes, so after execve i can free everything, and in case of pipes to close them.

if no output redirect, and only 1 cmd, i need no pipe, right?
if 
*/

int	check_cmd(char *cmd, t_cmd_table *cmd_struct)
{
	int		i;
	int		size;

	i = 0;
	size = ft_strlen(cmd_struct->str[0]);
	while (cmd[i] && cmd[i] != ' ')
		i++;
	if (ft_strncmp(cmd, cmd_struct->str[0], size) || i != size)
		return (0);
	return (1);
}

int	ft_execute_builtins(t_cmd_table *cmd, t_common *c)
{
	t_cmd_table	*tmp;

	tmp = cmd;
	if (!tmp)
		return (perror("Error initializing cmd\n"), EXIT_FAILURE);
	if (check_cmd("pwd", tmp))
		ft_pwd();
	else if (check_cmd("export", tmp))	
		ft_export(tmp->str, c->env);
	else if (check_cmd("env", tmp))
		ft_env(c->env);
	else if (check_cmd("exit", tmp))
		ft_exit(c, tmp->str);
	else if (check_cmd("unset", tmp))
		ft_unset(tmp->str, c);
	else if (check_cmd("echo", tmp))
		ft_echo(tmp->str);
	else if (check_cmd("cd", cmd))
		ft_cd(tmp->str, c);
	free(c->raw_prompt);
	return (EXIT_SUCCESS);
}

/* //check if it is a directory
int	check_if_dir(t_io_red *io)
{
	struct stat	s;

	if (io->type == REDIR_IN || io->type == HEREDOC)
	{
		if (stat(io->infile, &s) == 0)
		{
			if (S_ISDIR(s.st_mode))//check if this is allowed
				return (1);
			else
				return (0);
		}
	}
	else if (io->type == REDIR_OUT || io->type == APPEND)
	{
		if (stat(io->outfile, &s) == 0)
		{
			if (S_ISDIR(s.st_mode))//check if this is allowed
				return (1);
			else
				return (0);
		}
	}
	else
		return (-1);
} */

int	pipe_needed(t_cmd_table *cmd)
{
	if (cmd->read_fd != 0 || cmd->write_fd != 1)
		return (1);
	return (0);
}

int	create_pipe(t_pipe *new)
{
	if (new->read_fd != -1 || new->write_fd != -1)
		printf("Pipe not empty");
	if (pipe(new->pipes) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

// wenn mehrere infiles, muss es das letzte sein. aber es muss erst gecheckt werden ob beide exisiteren.
int	ft_execute(t_common *c)
{
	t_list		*curr_cmd;
	t_cmd_table	*curr_cmd_table;

	curr_cmd = c->cmd_struct;
	while (curr_cmd)
	{
		curr_cmd_table = curr_cmd->content;
		open_io(curr_cmd_table->io_red, curr_cmd_table);
		curr_cmd = curr_cmd->next;
	}
	curr_cmd = c->cmd_struct;
	while (curr_cmd)
	{
		if (pipe_needed(curr_cmd->content))
			create_pipe(&c->new_pipe);
		
		curr_cmd = curr_cmd->next;
	}
	return (EXIT_SUCCESS);
}

// ex.: < in cat | cat > out