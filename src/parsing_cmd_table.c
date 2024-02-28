/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_table.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:12:18 by caigner           #+#    #+#             */
/*   Updated: 2024/02/28 19:59:07 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

char	**setup_env(t_env *env)
{
	t_env	*tmp;
	char	**p;
	char	*s;
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
			s = ft_strjoin(env->variable, "=");
			p[i] = ft_strjoin(s, env->value);
			free (s);
		}
		tmp = tmp->next;
	}
	return (p);
}

int	cmd_to_node(t_token *token, t_cmd_table *cmd_node)
{
	int	i;
	t_token *tmp;

	tmp = token;
	i = 0;
	while (tmp)
	{
		if (tmp->type == VOID)
			i++;
		tmp = tmp->next;
	}
	cmd_node->str = malloc(sizeof(char*) * (i + 1));
	if (!cmd_node->str)
		return (perror("Error initializing str in cmd_to_node\n"), 1);
	i = 0;
	while (token)
	{
		if (token->type == VOID)
			cmd_node->str[i++] = token->data;
		token = token->next;
	}
	cmd_node->str[i] = NULL;
	return (EXIT_SUCCESS);
}

int	input_to_node(t_token *token, t_io_red *tmp, t_cmd_table *node)
{
	static int	i;
	char		*c;

	i = 0;
	if (token->type == HEREDOC)
	{
		tmp->heredoc_limiter = token->data;
		if (node->heredoc_name)
			free(node->heredoc_name);
		c = ft_itoa(i++);
		node->heredoc_name = ft_strjoin(".heredoc_tmp", c);
		free(c);
		if (!node->heredoc_name)
			return (perror("Error initializing str in input_to_node\n"), 1);
		tmp->infile = node->heredoc_name; //CHECK IF .heredoc_tmp already EXISTS, IF YES increment i
	}
	else
		tmp->infile = token->data;
	tmp->type = token->type;
	return (EXIT_SUCCESS);
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
	node->read_fd = 0;
	node->write_fd = 1;
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
	t_list	*tmp_tok;
	t_list	*tmp_cmd;
	char	**arr;
	char	**sub_arr;
	int		i;
	int		size;

	size = count_pipes(c->raw_prompt);
	arr = tokenize_one(c->raw_prompt, size);
	i = 0;
    while (i <= size)
    {
        sub_arr = prep_input(arr[i++]);
		tmp_tok = ft_lstnew(NULL);
		if (!tmp_tok)
			return (EXIT_FAILURE);
		add_to_list(sub_arr, tmp_tok);
		ft_lstadd_back(&c->tokens, tmp_tok);
//		printf("\n");
    }                             
	tmp_tok = c->tokens;
	tmp_cmd = ft_lstnew(malloc(sizeof(t_cmd_table *)));
	if (!tmp_cmd || !tmp_cmd->content)
		return (perror("Error initializing cmd_struct\n"), 1);
	while (tmp_tok)
	{
		printf("!!!!");
		token_to_struct(tmp_tok->content, tmp_cmd->content);
		ft_lstadd_back(&c->cmd_struct, tmp_cmd);
		if (tmp_tok->next)
		{
			tmp_cmd = ft_lstnew(malloc(sizeof(t_cmd_table *)));
			if (!tmp_cmd || !tmp_cmd->content)
				return (perror("Error initializing cmd_struct\n"), 1);
		}
		tmp_tok = tmp_tok->next;
	}
	//ft_expansion(c->cmd_struct);
	//rm_quotes(c->cmd_struct);
	return (EXIT_SUCCESS);
}
