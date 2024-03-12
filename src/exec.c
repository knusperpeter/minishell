/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:25:50 by chris             #+#    #+#             */
/*   Updated: 2024/03/12 15:45:09 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
/**
 * Function: check_cmd
 * Description: This function checks if the provided command matches the first string in the command structure.
 * Parameters: 
 * - cmd: A pointer to the command string to be checked.
 * - cmd_struct: A pointer to the command table structure, which contains the command strings.
 * Returns: 
 * - 1 if the command matches the first string in the command structure.
 * - 0 if the command does not match.
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
/**
 * Function: ft_builtins
 * Description: This function checks if the command in the command table matches any of the built-in commands 
 *              ("pwd", "export", "env", "exit", "unset", "echo", "cd"). If a match is found, the corresponding 
 *              built-in function is called.
 * Parameters: 
 * - cmd: A pointer to the command table structure, which contains the command to be executed.
 * - c: A pointer to the common structure, which contains the shell data.
 * Returns: 
 * - 1 if a built-in command was executed.
 * - 0 if the command does not match any built-in commands.
 * - EXIT_FAILURE if an error occurred during initialization.
 */
int	ft_builtins(t_cmd_table *cmd, t_common *c)
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
	else
	 	return (0);
	return (1);
}

void	safe_close(int *fd)
{
	if (*fd > 2)
		close(*fd);
	*fd = -1;
}

void	replace_fd(int *fd1, int *fd2)
{
	if (*fd1 == -1)
		return;
	safe_close(fd1);
	*fd2 = *fd1;
	*fd1 = -1;
}

void	safe_close_pipe(t_pipe *pipe)
{
	safe_close(pipe->read_fd);
	safe_close(pipe->write_fd);
}

void	handle_pipes_child(t_pipe *new, t_pipe *old)
{
	safe_close(new->read_fd);
	replace_fd(new->read_fd, old->read_fd);
}

void	handle_pipes_parent(t_pipe *new, t_pipe *old)
{
	safe_close(new->read_fd);
	replace_fd(new->read_fd, old->read_fd);
}

void	close_all_pipes(t_common *c)
{
	safe_close_pipe(&c->old_pipe);
	safe_close_pipe(&c->new_pipe);
}
/**
 * Function: create_pipe
 * Description: This function creates a new pipe. If the pipe already exists, it is first closed safely. 
 *              The file descriptors for reading and writing are then set.
 * Parameters: 
 * - new: A pointer to the pipe structure to be created.
 * Returns: 
 * - EXIT_SUCCESS if the pipe was successfully created.
 * - EXIT_FAILURE if an error occurred while creating the pipe.
 */
int	create_pipe(t_pipe *new)
{
	if (new->pipes[0] != -1 || new->pipes[1] != -1)
	{
		safe_close_pipe(new);
	//	printf("Pipe not empty");
	}
	if (pipe(new->pipes) == -1)
		return (EXIT_FAILURE);
	new->read_fd = &new->pipes[0];
	new->write_fd = &new->pipes[1];
	return (EXIT_SUCCESS);
}
/**
 * Function: close_fds
 * Description: Safely closes the file descriptors associated with the command and the pipes in the common structure.
 * Parameters: c - The common structure containing the shell data, cmd - The command table.
 */
void	close_fds(t_common *c, t_cmd_table *cmd)
{
	if (cmd->read_fd != -1)
		safe_close(&cmd->read_fd);
	if (cmd->write_fd != -1)
		safe_close(&cmd->write_fd);
	if (c->old_pipe.pipes[0] != -1)
	{
		safe_close(&c->old_pipe.pipes[1]);
		safe_close(&c->old_pipe.pipes[0]);
	}
	if (c->new_pipe.pipes[0] != -1)
	{
		safe_close(&c->new_pipe.pipes[1]);
		safe_close(&c->new_pipe.pipes[0]);
	}
}

int	get_env_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}
/**
 * Function: env_to_arr
 * Description: Converts the environment linked list into an array.
 * Parameters: size - The size of the environment linked list, env - The linked list of environment variables.
 * Returns: An array of environment variables.
 */
char	**env_to_arr(int size, t_env *env)
{
	char	**ret;
	char	*s;
	int		i;

	ret = malloc(sizeof(char *) * (size + 1));
	if (!ret)
		return (ft_printerrno(NULL), NULL);
	i = 0;
	while (i < size)
	{
		s = ft_strjoin(env->variable, "=");
		if (!s)
			return (ft_printerrno(NULL), NULL);
		ret[i] = ft_strjoin(s, env->value);
		free(s);
		if (!ret[i])
		{
			free_2d(ret);
			return (ft_printerrno(NULL), NULL);
		}
		i++;
		env = env->next;
	}
	ret[i] = NULL;
	return (ret);
}

char	**get_envp(t_env *env)
{
	int		size;
	char	**ret;
	t_env	*tmp;

	ret = NULL;
	tmp = env;
	size = get_env_size(tmp);
	tmp = env;
	if (size > 0)
	{
		ret = env_to_arr(size, env);
	}
	return (ret);
}
/**
 * Function: ft_preexec
 * Description: Prepares the execution of a command by setting up the file descriptors.
 * Parameters: cmd_table - The linked list of command tables, cmd - The command table, c - The common structure containing the shell data.
 */
void ft_preexec(t_list_d *cmd_table, t_cmd_table *cmd, t_common *c)
{
	cmd = cmd_table->content;
	if (cmd->read_fd != 0 && c->old_pipe.pipes[0] == -1)
	{
		if (dup2(cmd->read_fd, 0) == -1)
			ft_printerrno(strerror(errno));
	}
	else if (cmd->read_fd != 0)
	{
		if (dup2(c->old_pipe.pipes[0], 0) == -1)
			ft_printerrno(strerror(errno));
	}
	if (cmd_table->next && cmd->write_fd != 1)
	{
		if (dup2(c->new_pipe.pipes[1], 1) == -1)
			ft_printerrno(strerror(errno));			
	}
	else if (cmd->write_fd != 1)
	{
		if (dup2(c->new_pipe.pipes[1], 0) == -1)
			ft_printerrno(strerror(errno));
	}
	close_fds(c, cmd);
}

int	ft_exec_builtins(t_common *c, t_cmd_table *cmd)
{
	if (ft_builtins(cmd, c))
		return (1);
	else
		return (0);
}
/**
 * Function: ft_exec_cmd
 * Description: Executes a command, handling built-in commands, forking, and setting up pipes.
 * Parameters: c - The common structure containing the shell data, cmd_table - The linked list of command tables.
 * Returns: EXIT_SUCCESS upon successful execution.
 */
int	ft_exec_cmd(t_common *c, t_list_d *cmd_table)
{
	t_cmd_table	*cmd;

	cmd = cmd_table->content;
	c->envp = get_envp(c->env);
	if (!c->envp)
		ft_printerrno("c->envp: ");
	cmd->id = fork();
	if (cmd->id == 0)
	{
		ft_preexec(cmd_table, cmd, c);
		if (!ft_exec_builtins(c, cmd))
		{
			get_cmd_path(c, cmd);
			execve(cmd->exec_path, cmd->str, c->envp);
		}
	}
	if (cmd_table->next)
		safe_close(&c->new_pipe.pipes[1]);
	else
		safe_close(&cmd->write_fd);
	return (EXIT_SUCCESS);
}
/**
 * Function: wait_all_childs
 * Description: Waits for all child processes to finish execution.
 * Parameters: c - The common structure containing the shell data.
 * This function iterates over the command structure linked list and waits for each child process.
 */
void	wait_all_childs(t_common *c)
{
	t_list_d	*tmp;
	t_cmd_table	*curr;
	
	tmp = ft_lstlast_d(c->cmd_struct);
	while (tmp)
	{
		curr = tmp->content;
		waitpid(curr->id, NULL, 0);
		tmp = tmp->prev;
	}
}

/**
 * Function: ft_execute
 * Description: This function executes all the commands in the command table linked list. It first opens the I/O redirections 
 *              for each command, then creates a pipe if there are multiple commands. Each command is then executed in order. 
 *              After all commands have been executed, all pipes are closed and the function waits for all child processes to finish.
 * Parameters: 
 * - c: A pointer to the common structure, which contains the shell data.
 * Returns: 
 * - EXIT_SUCCESS upon successful execution of all commands.
 */

// wenn mehrere infiles, muss es das letzte sein. aber es muss erst gecheckt werden ob beide exisiteren.
int	ft_execute(t_common *c)
{
	t_list_d		*curr_cmd;
	t_cmd_table		*curr_cmd_table;
	int				pipes;

	pipes = -1;
	curr_cmd = c->cmd_struct;
	while (curr_cmd)
	{
		curr_cmd_table = curr_cmd->content;
		open_io(curr_cmd_table->io_red, curr_cmd_table);
		pipes += 1;
		curr_cmd = curr_cmd->next;
	}
	if (pipes >  0)
	{
		if (create_pipe(&c->new_pipe))
			ft_printerrno("pipe :");
		curr_cmd = c->cmd_struct;
		while (curr_cmd)
		{
			curr_cmd_table = curr_cmd->content;
			if (curr_cmd_table->str[0])
				ft_exec_cmd(c, curr_cmd);
			curr_cmd = curr_cmd->next;
		}
		close_all_pipes(c);
		wait_all_childs(c);
	}
	else if ( pipes == 0)
	{
		curr_cmd_table = c->cmd_struct->content;
		if (curr_cmd_table->str[0])
			ft_exec_cmd(c, c->cmd_struct);
		
	}
	return (EXIT_SUCCESS);
}

// ex.: < in cat | cat > out
