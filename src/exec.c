/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:25:50 by chris             #+#    #+#             */
/*   Updated: 2024/02/21 13:40:47 by caigner          ###   ########.fr       */
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

void	here_doc(char *limiter, t_cmd_table *cmd_table)
{
	char	*buf;

	if (cmd_table->read_fd == -1)
		printf("pipex: %s: %s\n", limiter, strerror(errno));
	while (1)
	{
		write(1, "minishell heredoc> ", 15);
		if (get_next_line(0, &buf, 0) < 0)
			exit (1);
		if (buf == NULL || *buf == '\0')
			break ;
		if (ft_strncmp(limiter, buf, ft_strlen(limiter)) == 0)
			break ;
		write(cmd_table->read_fd, buf, ft_strlen(buf));
	}
	get_next_line(0, &buf, 1);
	free(buf);
	close(cmd_table->read_fd);
	cmd_table->read_fd = open(".heredoc_tmp", O_RDONLY);
	if (cmd_table->read_fd == -1)
	{
		unlink(".heredoc_tmp");
		printf("minishell: %s\n", strerror(errno));
	}
}

int	open_file(t_token *token, t_cmd_table *cmd_table)
{
	if (token->type == HEREDOC)
	{
		cmd_table->read_fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		here_doc(token->data, cmd_table);
	}
	else if (token->type == REDIR_IN)
	{
		cmd_table->read_fd = open(token->data, O_RDONLY);
		if (cmd_table->read_fd == -1)
			printf("minishell: %s: %s\n", token->data, strerror(errno));
	}
	else if (token->type == REDIR_OUT)
	{
		cmd_table->write_fd = open(token->data, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR
				| S_IWUSR);
	}
	else if (token->type == APPEND)
	{
		cmd_table->write_fd = open(token->data, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR
				| S_IWUSR);
	}
	if (cmd_table->write_fd == -1)
		printf("minishell: %s: %s\n", token->data, strerror(errno));
	return (EXIT_SUCCESS);
}

void	open_in_out(t_token *token, t_common *c)
{
	if (token->type == REDIR_IN || token->type == REDIR_OUT
			|| token->type == APPEND || token->type == HEREDOC)
		open_file(c->tokens->content, c->cmd_struct->content);
}

void	setup_env(t_common *c, t_cmd_table *cmd_table)
{
	t_env	*tmp;

	tmp = c->env;
	
}

int	prepare_execution(t_common *c)
{
	t_list	*tmp_tok;
	t_list	*tmp_cmd;
	
	tmp_tok = c->tokens;
	while (tmp_tok)
	{
		setup_env(c, tmp_cmd->content);
		//expand_variable(c->tokens->content, c->env);
		open_in_out(tmp_tok->content, c);
		add_cmd(c->cmd_struct->content);
		tmp_tok = tmp_tok->next;
	}
	return (EXIT_SUCCESS);
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
int	ft_exec(t_common *c)
{
	init_loop_data(c);
	c->raw_prompt = prompt();
	if (c->raw_prompt[0])
	{
		//lexing&parsing
		prepare_execution(c);
		ft_execute_builtins(c->cmd_struct->content, c);
	}
	return (0);
}
