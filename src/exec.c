/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:25:50 by chris             #+#    #+#             */
/*   Updated: 2024/02/23 16:45:32 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
/*
if i only have one cmd, do i still need to fork? 
	->probably yes, so after execve i can free everything, and in case of pipes to close them.

if no output redirect, and only 1 cmd, i need no pipe, right?
if 
*/
 
char	*prompt(void)
{
	char	*line;

	line = readline("minishell🔮: 🚬🦦❯ "); // check rl_redisplay
	if (ft_strlen(line) > 0)
		add_history(line);
	return (line);
}

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

void	init_loop_data(t_common *c){
	c->raw_prompt = NULL;
	c->tokens = NULL;
	c->cmd_struct = NULL;
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

// ex.: < in cat | cat > out
int	ft_loop(t_common *c)
{
	init_loop_data(c);
	c->raw_prompt = prompt();
	if (c->raw_prompt[0])
	{
		//lexing&parsing
		ft_parsing(c);
//		ft_execute(c);
//		ft_execute_builtins(c->cmd_struct->content, c);
	}
	return (0);
}
