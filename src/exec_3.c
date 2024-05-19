/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 20:57:43 by caigner           #+#    #+#             */
/*   Updated: 2024/05/19 16:44:31 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_fds_parent(t_common *c, int *fd)
{
	if (c->cmd_count > 1)
		close(fd[1]);
	if (c->old_pipe)
		close(c->old_pipe);
	if (c->cmd_count > 1)
		c->old_pipe = fd[0];
}

void	ft_redirect_io(t_common *c, t_cmd_table *cmd, int curr, int *fd)
{
	if (cmd->read_fd == -1 || cmd->write_fd == -1)
		ft_putstr_fd("minishell: error opening file\n", 2);
	if (cmd->read_fd != STDIN)
	{
		if (dup2(cmd->read_fd, STDIN) == -1)
			ft_printerrno("1\n");
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
	close_fds(c, fd, cmd);
}

int	is_builtin(char *cmd)
{
	int	size;

	if (!cmd)
		return (0);
	size = ft_strlen(cmd);
	if (ft_strncmp("echo", cmd, 5) == 0 || \
		ft_strncmp("env", cmd, 4) == 0 || \
		ft_strncmp("unset", cmd, 6) == 0 || \
		ft_strncmp("export", cmd, 7) == 0 || \
		ft_strncmp("cd", cmd, 3) == 0 || \
		ft_strncmp("pwd", cmd, 4) == 0 || \
		ft_strncmp("exit", cmd, 5) == 0)
		return (1);
	return (0);
}

void	wait_all_childs(t_common *c)
{
	t_list_d	*tmp;
	t_cmd_table	*curr;

	tmp = c->cmd_struct;
	while (tmp)
	{
		curr = tmp->content;
		waitpid(curr->id, (int *)&c->exitstatus, 0);
		if (WIFEXITED(c->exitstatus))
			c->exitstatus = WEXITSTATUS(c->exitstatus);
		else if (WIFSIGNALED(c->exitstatus))
			c->exitstatus = 128 + WTERMSIG(c->exitstatus);
		tmp = tmp->next;
	}
}

int	ft_count_cmds(t_list_d *cmd_struct)
{
	int			count;
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
