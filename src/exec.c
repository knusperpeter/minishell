/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:25:50 by chris             #+#    #+#             */
/*   Updated: 2024/03/08 22:16:56 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
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
//	if (c->envp)
//		free_2d(c->envp);
	if (cmd_table->next)
		safe_close(&c->new_pipe.pipes[1]);
	else
		safe_close(&cmd->write_fd);
	return (EXIT_SUCCESS);
}

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
