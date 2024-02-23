/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_table.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:12:18 by caigner           #+#    #+#             */
/*   Updated: 2024/02/23 16:45:38 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <iterator>
#include <stdlib.h>

char	**setup_env(t_env *env)
{
	t_env	*tmp;
	char	**p;
	int		i;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	p = malloc(sizeof(char *) * i);
	if (!p)
		return (perror("malloc error\n"), NULL);
	i = 0;
	tmp = env;
	while (tmp)
	{
		if (!tmp->flag)
		{
			p[i] = ft_strjoin(env->variable, "=");
			p[i] = ft_strjoin(p[i], env->value);
		}
		tmp = tmp->next;
	}
}

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
	static int	i; //die müsste wsl in parent function stehen?
	char		*buf;

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
//add return value?
//ACHTUNG: ich kann erst öffnen, wenn expanded wurde!!!
//erst expanden, dann mit cmd_table durchiterieren
/* void	open_file(t_token *token, t_cmd_table *cmd_node)
{
	if (token->type == HEREDOC)
	{
		cmd_node->read_fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		here_doc(token->data, cmd_node);
	}
	else if (token->type == REDIR_IN)
	{
		cmd_node->read_fd = open(token->data, O_RDONLY);
		if (cmd_node->read_fd == -1)
			printf("minishell: %s: %s\n", token->data, strerror(errno));
	}
	else if (token->type == REDIR_OUT)
	{
		cmd_node->write_fd = open(token->data, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR
				| S_IWUSR);
	}
	else if (token->type == APPEND)
	{
		cmd_node->write_fd = open(token->data, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR
				| S_IWUSR);
	}
	if (cmd_node->write_fd == -1)
		printf("minishell: %s: %s\n", token->data, strerror(errno));
} */

t_token	*cmd_to_node(t_token *token, t_cmd_table *cmd_node)
{
	int	i;
	t_token *tmp;

	tmp = token;
	i = 0;
	while (tmp)
	{
		if (tmp->type == VOID || tmp->type == STRING)
			i++;
		tmp = tmp->next;
	}
	cmd_node->str = malloc(sizeof(char*) * (i + 1));
	if (!cmd_node->str)
		return (perror("Error initializing char * in cmd_to_node\n"), NULL);//evtl token returnen?
	i = 0;
	while (token)
	{
		if (token->type == VOID || token->type == STRING)
			cmd_node->str[i++] = token->data;
		token = token->next;
	}
	cmd_node->str[i] = NULL;
	return (token);
}

int	input_to_node(t_token *token, t_io_red *tmp, t_cmd_table *node)
{
	static int	i;

	i = 0;
	if (token->type == HEREDOC)
	{
		tmp->heredoc_limiter = token->data;
		node->heredoc_name = ft_strjoin(".heredoc_tmp", ft_itoa(i++));
		if (!node->heredoc_name)
			return (perror("Error initializing str in input_to_node\n"), 1);
	}
	else
		tmp->infile = token->data;
	tmp->type = token->type;
}

int	red_to_node(t_token *token, t_cmd_table *node)
{
	t_list		*red_node;
	t_io_red	*tmp;

	red_node = ft_lstnew(malloc(sizeof(t_io_red *)));
	if (!node->io_red || !node->io_red->content)
		return (perror("Error initializing t_list in red_to_node\n"), 1);
	tmp = red_node->content;
	if(token->type == REDIR_IN || token->type == HEREDOC)
		input_to_node(token, tmp, node);
	else
	{
		tmp->type = token->type;
		tmp->outfile = token->data;
	}
	ft_lstadd_back(&node->io_red, red_node);
	return (EXIT_SUCCESS);
}

void	init_cmd_table(t_cmd_table *node)
{
	node->io_red = NULL;
	node->heredoc_name = NULL;
	node->str = NULL;
	node->exec_path = NULL;
}

void	token_to_struct(t_token *token, t_cmd_table *cmd_node)
{
	static int	i;
	t_token	*tmp;
	
	i = 0;
	tmp = token;
	init_cmd_table(cmd_node);
	cmd_to_node(token, cmd_node);
	while (tmp)
	{
		if (tmp->type == REDIR_IN || tmp->type == REDIR_OUT
				|| tmp->type == APPEND || tmp->type == HEREDOC)
			red_to_node(tmp, cmd_node);
		tmp = tmp->next;
	}
}

int	ft_parsing(t_common *c)
{
	//Michis part vom parsen gehört hier davor
	t_list	*tmp_tok;
	t_list	*tmp_cmd;
	
	tmp_tok = c->tokens;
	tmp_cmd = c->cmd_struct;
	while (tmp_tok)
	{
		//expand_variable(c->tokens->content, c->env);
		token_to_struct(tmp_tok->content, tmp_cmd->content);
		//add_cmd(c->cmd_struct->content);
		tmp_tok = tmp_tok->next;
	}
	return (EXIT_SUCCESS);
}
