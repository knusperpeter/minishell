/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:25:50 by chris             #+#    #+#             */
/*   Updated: 2024/02/24 18:32:08 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
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
	cmd->str = ft_split(c->raw_prompt, ' ');
	if (!c->cmd_struct->content)
		return (perror("Error initializing simple_cmd\n"), EXIT_FAILURE);
	if (check_cmd("pwd", c->cmd_struct->content))
		ft_pwd();
	if (check_cmd("export", c->cmd_struct->content))	
		ft_export(cmd->str, c->env);
	if (check_cmd("env", c->cmd_struct->content))
		ft_env(c->env);
	if (check_cmd("exit", c->cmd_struct->content))
		ft_exit(c, cmd->str);
	if (check_cmd("unset", c->cmd_struct->content))
		ft_unset(cmd->str, c);
	if (check_cmd("echo", c->cmd_struct->content))
		ft_echo(cmd->str);
	if (check_cmd("cd", c->cmd_struct->content))
		ft_cd(cmd->str, c);
	free_2d(cmd->str);
	free(c->raw_prompt);
	return (EXIT_SUCCESS);
}

// if the filename is valid
// check if the file is a directory: "cat: -: Is a directory"
int	check_redirections(t_cmd_table *cmd_table)
{
	t_list		*curr_io;
	t_io_red	*io;

	curr_io = cmd_table->io_red->content;
	while (curr_io)
	{
		io = curr_io->content;
		if (io->type == HEREDOC || io->type == REDIR_IN)
			check_io_name(io->infile);
		else if (io->type == APPEND || io->type == REDIR_OUT)
			check_io_name(io->outfile);
		
//////////////////////////////////////////////	Show must go on here-> implement function: check_io_name
	}
	return (EXIT_SUCCESS);
}

int	ft_execute(t_common *c)
{
	t_list		*curr_cmd;
	t_cmd_table	*curr_cmd_table;

	curr_cmd = c->cmd_struct;
	while (curr_cmd)
	{
		curr_cmd_table = curr_cmd->content;
		if (!check_redirections(curr_cmd->content))
			open_io(curr_cmd_table->io_red, curr_cmd_table);
		curr_cmd = curr_cmd->next;
	}
	return (EXIT_SUCCESS);
}

// ex.: < in cat | cat > out

