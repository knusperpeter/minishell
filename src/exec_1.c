/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:25:50 by chris             #+#    #+#             */
/*   Updated: 2024/05/19 16:42:15 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_check_builtin(t_cmd_table *cmd)
{
	if (cmd->str[0])
	{
		if ((!ft_strncmp("export", cmd->str[0], 7)
				&& cmd->str[1])
			|| !ft_strncmp("exit", cmd->str[0], 5)
			|| !ft_strncmp("unset", cmd->str[0], 6)
			|| !ft_strncmp("cd", cmd->str[0], 3))
			return (1);
	}
	return (0);
}

void	prepare_execution(t_common *c, t_cmd_table *table, int curr, int *fd)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (!open_redirections(c, table))
		return (close_fds(c, fd, table), ft_clean_exit(c, NULL, 1));
	ft_redirect_io(c, table, curr, fd);
	if (!table->str[0])
		ft_clean_exit(c, NULL, 1);
}

void	execute_child(t_common *c, t_cmd_table *table, int curr, int *fd)
{
	prepare_execution(c, table, curr, fd);
	if (is_builtin(table->str[0]) && table->permission)
	{
		ft_builtins(table, c);
		ft_clean_exit(c, NULL, 1);
	}
	else if (table->permission)
	{
		c->envp = get_envp(c, c->env);
		if (get_cmd_path(c, table))
		{
			execve(table->exec_path, table->str, c->envp);
			c->exitstatus = 127;
			perror(table->str[0]);
		}
		else
		{
			c->exitstatus = 127;
			cmd_not_found(table->str[0]);
		}
	}
	else
		c->exitstatus = 1;
	ft_clean_exit(c, NULL, 1);
}

int	execute_cmds(t_common *c)
{
	int			curr;
	int			fd[2];
	t_list_d	*curr_cmd;
	t_cmd_table	*curr_cmd_table;

	curr = 1;
	c->old_pipe = 0;
	curr_cmd = c->cmd_struct;
	while (curr <= c->cmd_count)
	{
		curr_cmd_table = curr_cmd->content;
		if (curr < c->cmd_count)
			if (pipe(fd) == -1)
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
	else
	{
		status = execute_cmds(c);
		wait_all_childs(c);
	}
	if (status == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
