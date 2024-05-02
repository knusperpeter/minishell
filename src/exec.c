/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:25:50 by chris             #+#    #+#             */
/*   Updated: 2024/05/02 16:18:31 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * Checks if the first word in the command string matches the first string in the command structure.
 * @param cmd: The command string, cmd_struct: The command structure.
 * @return: 1 if they match exactly, 0 otherwise.
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
 * Executes built-in commands if they match the first word in the command table.
 * @param cmd: The command table, c: Common structure.
 * @return: 1 if a built-in command was executed, 0 otherwise, or EXIT_FAILURE on error.
 */
int	ft_builtins(t_cmd_table *cmd, t_common *c)
{
	t_cmd_table	*tmp;

	tmp = cmd;
	if (!tmp)
		return (perror("Error initializing cmd\n"), EXIT_FAILURE);
	if (check_cmd("pwd", tmp))
		/* c->exitstatus =  */ft_pwd();
	else if (check_cmd("export", tmp))
		c->exitstatus = ft_export(tmp->str, c->env);
	else if (check_cmd("env", tmp))
		/* c->exitstatus =  */ft_env(c->env);
	else if (check_cmd("exit", tmp))
		ft_exit(c, tmp->str);
	else if (check_cmd("unset", tmp))
		/* c->exitstatus =  */ft_unset(tmp->str, c);
	else if (check_cmd("echo", tmp))
		/* c->exitstatus =  */ft_echo(tmp->str);
	else if (check_cmd("cd", cmd))
		c->exitstatus = ft_cd(tmp->str, c);
	else
	 	return (0);
	return (1);
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

/**
 * Converts the linked list of environment variables to an array.
 * @param env: The head of the linked list of environment variables.
 * @return: An array of strings, each representing an environment variable, or NULL if an error occurs.
 */
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
 * Closes file descriptors that are no longer needed.
 * @param fd: Array of file descriptors, prev: Previous file descriptor, cmd: Command table.
 * Closes read and write file descriptors in the command table, previous descriptor, and both descriptors in the array.
 */
void	close_fds(t_common *c, int *fd, t_cmd_table *cmd)
{	
	if (cmd->read_fd != STDIN)
		close(cmd->read_fd);
	if (cmd->write_fd != STDOUT)
		close(cmd->write_fd);
	if (c->old_pipe)
		close(c->old_pipe);
	if (c->cmd_count > 1)
		close(fd[0]); //if more than 1 cmd
	if (c->cmd_count > 1)
		close(fd[1]); //if more than 1 cmd
}

/**
 * Handles file descriptors for the parent process in a pipe.
 * @param c: Common structure, fd: File descriptors.
 * Closes the write end of the current pipe and the read end of the old pipe if they exist.
 */
void	handle_fds_parent(t_common *c, int *fd)
{
	if (c->cmd_count > 1)
		close(fd[1]);
	if (c->old_pipe)
		close(c->old_pipe);
	if (c->cmd_count > 1)
		c->old_pipe = fd[0];
}

/**
 * Redirects input/output based on the command table and current command index.
 * @param c: Common structure, cmd: Command table, i: Current command index, fd: File descriptors, old_pipe: Previous pipe.
 * Handles error checking and redirection for both input and output.
 */
void ft_redirect_io(t_common *c, t_cmd_table *cmd, int curr, int *fd)
{
	if (cmd->read_fd == -1 || cmd->write_fd == -1)
		dprintf(2, "error opening file");
	if (cmd->read_fd != STDIN)
	{
		if (dup2(cmd->read_fd, STDIN) == -1)
			ft_printerrno("1\n"); //check error and exit 
	}
	else if (c->old_pipe != 0 && c->cmd_count != 1)
	{
		if (dup2(c->old_pipe, STDIN) == -1)
			ft_printerrno("2\n");
	}
	if (cmd->write_fd != STDOUT)
	{
		if (dup2(cmd->write_fd, STDOUT) == -1)
			ft_printerrno("3");
	}
	else if (curr < c->cmd_count && c->cmd_count != 1)
	{
		if (dup2(fd[1], STDOUT) == -1)
			ft_printerrno("4");
	}
	close_fds(c, fd, cmd);// if builtin, do i need to make it different?
}

/**
 * Checks if the given command is a built-in shell command.
 * @param cmd: The command to check.
 * @return: 1 if the command is a built-in, 0 otherwise.
 */
int	is_builtin(char *cmd)
{
	int	size;
	
	if (!cmd)
		return (0);
	size = ft_strlen(cmd);
	if (ft_strncmp("echo", cmd, size) == 0 || \
		ft_strncmp("env", cmd, size) == 0 || \
		ft_strncmp("unset", cmd, size) == 0 || \
		ft_strncmp("export", cmd, size) == 0 || \
		ft_strncmp("cd", cmd, size) == 0 || \
		ft_strncmp("pwd", cmd, size) == 0 || \
		ft_strncmp("exit", cmd, size) == 0)
		return (1);
	return (0);
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
	int			wstatus;

	// change to from left to right
	tmp = c->cmd_struct;
	while (tmp)
	{
		curr = tmp->content;
		waitpid(curr->id, &wstatus, 0);
		if(WIFEXITED(c->exitstatus))
			c->exitstatus = WEXITSTATUS(c->exitstatus);
		else if(WIFSIGNALED(c->exitstatus))
			c->exitstatus = 128 + WTERMSIG(c->exitstatus);
		tmp = tmp->next;
	}
}

int	ft_count_cmds(t_list_d *cmd_struct)
{
	int		count;
	t_list_d	*tmp;

	count = 0;
	tmp = cmd_struct;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

int	ft_check_builtin(t_cmd_table *cmd)
{
	if ((!ft_strncmp("export", cmd->str[0], ft_strlen(cmd->str[0])) && cmd->str[1])
		|| !ft_strncmp("exit", cmd->str[0], ft_strlen(cmd->str[0]))
		|| !ft_strncmp("unset", cmd->str[0], ft_strlen(cmd->str[0]))
		|| !ft_strncmp("cd", cmd->str[0], ft_strlen(cmd->str[0])))
		return (1);
	return (0);
}

void	execute_child(t_common *c, t_cmd_table *curr_cmd_table, int curr, int *fd)
{
	if (!open_io(c, curr_cmd_table->io_red, curr_cmd_table))
		ft_clean_exit(c, NULL, 1);
	ft_redirect_io(c, curr_cmd_table, curr, fd);
	if (is_builtin(curr_cmd_table->str[0]))
	{
		ft_builtins(curr_cmd_table, c);
		ft_clean_exit(c, NULL, 1);
	}
	else
	{
		c->envp = get_envp(c->env);
		if (get_cmd_path(c, curr_cmd_table))
			execve(curr_cmd_table->exec_path, curr_cmd_table->str, c->envp);
		perror(curr_cmd_table->str[0]);
	}
	ft_clean_exit(c, NULL, 1);
}

int	execute_cmds(t_common *c)
{
	int	curr;
	int	fd[2];
	t_list_d *curr_cmd;
	t_cmd_table *curr_cmd_table;
	
	curr = 1;
	c->old_pipe = 0;
	curr_cmd = c->cmd_struct;
	while (curr <= c->cmd_count)
	{
		curr_cmd_table = curr_cmd->content;
		if (curr < c->cmd_count)
			if (pipe(fd) == -1) //if more than 1 cmd -jakob //since last needs no pipe, maybe better like this? -chris
				return (ft_printerrno("pipe: "), EXIT_FAILURE);
		curr_cmd_table->id = fork();
		if (curr_cmd_table->id == -1)
			return (ft_printerrno("fork: "), EXIT_FAILURE);
		else if (!curr_cmd_table->id)
			execute_child(c, curr_cmd_table, curr, fd);
		handle_fds_parent(c, fd);
		curr++;
		curr_cmd = curr_cmd->next;
	}
//	close(c->old_pipe); // close c->old_pipe? why not :(
	return (EXIT_SUCCESS);
}

int	ft_execute(t_common *c)
{
	int	status;
	
	c->cmd_count = ft_count_cmds(c->cmd_struct);
	if (c->cmd_count == 1 && ft_check_builtin(c->cmd_struct->content))
	{
		ft_builtins(c->cmd_struct->content, c);
		return (EXIT_SUCCESS);
	}
	else//
	{//
		status = execute_cmds(c);
		wait_all_childs(c);
	}//
	if (status == EXIT_FAILURE)
		return(EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
