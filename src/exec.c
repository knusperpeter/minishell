/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:25:50 by chris             #+#    #+#             */
/*   Updated: 2024/03/21 23:29:08 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
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
	if (*fd < 0)
		return;
//	if (!fd)
//		return;
	// if (*fd > 2)
	// {	
	close(*fd);
	*fd = -1;
	//}
}

void	replace_fd(int *fd1, int *fd2)
{
//	if (!fd1 || !fd2)
//		return;
	if (*fd1 == -1)
		return;
	safe_close(fd2);
	*fd2 = *fd1;
	*fd1 = -1;
}

void	safe_close_pipe(t_pipe *pipe)
{
	safe_close(&pipe->pipes[0]);
	safe_close(&pipe->pipes[1]);
}

void	handle_pipes_child(t_pipe *new, t_pipe *old)
{
	safe_close(&new->pipes[0]);
	replace_fd(&new->pipes[1], &old->pipes[1]);
}

void	handle_pipes_parent(t_pipe *new, t_pipe *old)
{
	safe_close(&new->pipes[1]);
	replace_fd(&new->pipes[0], &old->pipes[0]);
}

void	close_all_pipes(t_common *c)
{
	safe_close_pipe(&c->old_pipe);
	safe_close_pipe(&c->new_pipe);
}

/**
 * Function: close_fds
 * Description: Safely closes the file descriptors associated with the command and the pipes in the common structure.
 * Parameters: c - The common structure containing the shell data, cmd - The command table.
 */
void	close_fds(int *fd, int prev, t_cmd_table *cmd)
{
//	dprintf(2, "read: %d write: %d read_pipe: %d write_pipe: %d ild_pipe : %d\n", cmd->read_fd, cmd->write_fd, *(c->new_pipe.read_fd), *(c->new_pipe.write_fd), *(c->old_pipe.read_fd));
	// if (cmd->read_fd != -1)
	// 	safe_close(&cmd->read_fd);
	// if (cmd->write_fd != -1)
	// 	safe_close(&cmd->write_fd);

	
	//close fds if not STDIN || STDOUT
	(void)cmd;
	if (prev)
	{
//		safe_close(&c->old_pipe.pipes[1]);
		//safe_close(&prev);
		close(prev);
	}
	close(fd[0]); //if more than 1 cmd
	close(fd[1]); //if more than 1 cmd
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
void ft_redirect_io(t_common *c, t_cmd_table *cmd, int i, int *fd, int old_pipe)
{
	if (cmd->read_fd == -1 || cmd->write_fd == -1) // check if really -1 if wrong
		printf("error opening file");
	
	if (cmd->read_fd != STDIN)
	{
		dprintf(2, "lol1");
		if (dup2(cmd->read_fd, STDIN) == -1)
			ft_printerrno("1\n"); //check error and exit 
	}
	else if (fd[0] != -1 && c->cmd_count != 1)
	{
		dprintf(2, "lol2");
		if (dup2(old_pipe, STDIN) == -1)
			ft_printerrno("2\n");
	}
	if (cmd->write_fd != STDOUT)
	{
		dprintf(2, "lol3");
		dprintf(2, "redir\n");
		if (dup2(cmd->write_fd, STDOUT) == -1)
			ft_printerrno("3");
	}
	else if (i < c->cmd_count - 1 && c->cmd_count != 1)
	{
		dprintf(2, "lol4");
		if (dup2(fd[1], STDOUT) == -1)
			ft_printerrno("4");
	}
	close_fds(fd, old_pipe, cmd);
}

int	is_builtin(char *cmd)
{
	int	size;
	
	if (!cmd)
		return (0);
	size = ft_strlen(cmd);
	if (ft_strncmp(cmd, "echo", size) == 0 || \
		ft_strncmp(cmd, "env", size) == 0 || \
		ft_strncmp(cmd, "unset", size) == 0 || \
		ft_strncmp(cmd, "export", size) == 0 || \
		ft_strncmp(cmd, "cd", size) == 0 || \
		ft_strncmp(cmd, "pwd", size) == 0 || \
		ft_strncmp(cmd, "exit", size) == 0)
		return (1);
	return (0);
}

int	is_cmd_in_pipeline(t_list_d *cmd)
{
	t_cmd_table	*tmp;
	
	if (!cmd)
		return 0;
	if (cmd->prev || cmd->next)
	{
		if (cmd->prev)
			tmp = cmd->prev->content;
		else
			tmp = cmd->next->content;
		if (tmp->str && tmp->str[0])
			return (1);
	}
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
	tmp = ft_lstlast_d(c->cmd_struct);
	while (tmp)
	{
		curr = tmp->content;
		waitpid(curr->id, &wstatus, 0);
		if(WIFEXITED(c->exitstatus))
			c->exitstatus = WEXITSTATUS(c->exitstatus);
		else if(WIFSIGNALED(c->exitstatus))
			c->exitstatus = 128 + WTERMSIG(c->exitstatus);
		tmp = tmp->prev;
	}
}

int	ft_count_cmds(t_list_d *cmd_struct)
{
	int		count;
//	t_cmd_table	*curr;
	t_list_d	*tmp;

	count = 0;
	tmp = cmd_struct;
	while (tmp)
	{
//		curr = tmp->content;
//		if (curr->str && curr->str[0])
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
		//add cd and unset
	return (0);
}

int	wait_process(t_common *c, pid_t	id)
{
	int	wstatus;
	
	if (waitpid(id, &wstatus, 0) == -1)
		return (0);
	c->exitstatus = 12345677; /////////////// was?
	return (0);
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
	int				i;
	int fd[2];
	int prv_pipe;

	prv_pipe = 0;
	c->cmd_count = ft_count_cmds(c->cmd_struct);
	if (c->cmd_count == 1 && ft_check_builtin(c->cmd_struct->content))
	{
		ft_builtins(c->cmd_struct->content, c);
		return (EXIT_SUCCESS);
	}
	i = 0;
	curr_cmd = c->cmd_struct;
	while (i <= c->cmd_count - 1)
	{
		curr_cmd_table = curr_cmd->content;
		if (pipe(fd) == -1) //if more than 1 cmd
			return (ft_printerrno("pipe: "), EXIT_FAILURE);
		curr_cmd_table->id = fork();
		if (curr_cmd_table->id == -1)
			return (ft_printerrno("fork: "), EXIT_FAILURE);
		else if (!curr_cmd_table->id)
		{
			if (!open_io(curr_cmd_table->io_red, curr_cmd_table))
				printf("Error opening io\n"); //exit
			ft_redirect_io(c, curr_cmd_table, i, fd, prv_pipe);
			dprintf(2,"asdadadadsa\n");
			if (is_builtin(curr_cmd_table->str[0]))
				ft_builtins(curr_cmd_table, c); // free & exit
			else
			{
				c->envp = get_envp(c->env);
				if (get_cmd_path(c, curr_cmd_table)){
					dprintf(2, "%s\n", curr_cmd_table->exec_path);
					execve(curr_cmd_table->exec_path, curr_cmd_table->str, c->envp);
				}
				//free error and exit
				perror("execve");
			}
		}
		//safe_close(&curr_cmd_table->read_fd);
		//safe_close(&curr_cmd_table->write_fd);
		close(fd[1]); //if more than 1 cmd
		if (prv_pipe)
			close(prv_pipe);
		prv_pipe = fd[0]; //if more than 1 cmd
		// handle_pipes_parent(&c->new_pipe, &c->old_pipe);
//		wait_process(c, curr_cmd_table->id);
		i++;
		curr_cmd = curr_cmd->next;
	}
	// close prv_pipe?
	// close_all_pipes(c);
	wait_all_childs(c);
	return (0);
}

// ex.: < in cat | cat > out
