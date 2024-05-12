/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 20:57:47 by caigner           #+#    #+#             */
/*   Updated: 2024/05/13 01:30:04 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		ft_pwd(c, tmp->str);
	else if (check_cmd("export", tmp))
		ft_export(c, tmp->str, c->env);
	else if (check_cmd("env", tmp))
		c->exitstatus = ft_env(tmp->str, c->env);
	else if (check_cmd("exit", tmp) && c->cmd_count == 1)
		ft_exit(c, tmp->str);
	else if (check_cmd("unset", tmp))
		ft_unset(tmp->str, c);
	else if (check_cmd("echo", tmp))
		ft_echo(c, tmp->str);
	else if (check_cmd("cd", cmd))
		c->exitstatus = ft_cd(tmp->str, c);
	else
		return (0);
	return (1);
}

char	**env_to_arr(t_common *c, int size, t_env *env)
{
	char	**ret;
	char	*s;
	int		i;

	ret = ft_protect(c, malloc(sizeof(char *) * (size + 1)), 0, 0);
	i = 0;
	while (i < size)
	{
		s = ft_protect(c, ft_strjoin(env->variable, "="), ret, 0);
		ret[i] = ft_protect(c, ft_strjoin(s, env->value), s, ret);
		free(s);
		i++;
		env = env->next;
	}
	ret[i] = NULL;
	return (ret);
}

char	**get_envp(t_common *c, t_env *env)
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
		ret = env_to_arr(c, size, env);
	}
	return (ret);
}

void	close_fds(t_common *c, int *fd, t_cmd_table *cmd)
{
	if (cmd->read_fd != STDIN)
		close(cmd->read_fd);
	if (cmd->write_fd != STDOUT)
		close(cmd->write_fd);
	if (c->old_pipe)
		close(c->old_pipe);
	if (c->cmd_count > 1)
		close(fd[0]);
	if (c->cmd_count > 1)
		close(fd[1]);
}
