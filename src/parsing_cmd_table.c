/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_table.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:12:18 by caigner           #+#    #+#             */
/*   Updated: 2024/03/11 17:03:32 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
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

int	cmd_to_node(t_cmd_table *cmd_node)
{
	int			i;
	t_list		*cmd_tok;
	t_cmd_table *cmd_tmp;

	cmd_tmp = cmd_node;
	cmd_tok = cmd_tmp->cmds;
	i = 0;
	while (cmd_tok)
	{
		i++;
		cmd_tok = cmd_tok->next;
	}
	cmd_tmp->str = malloc(sizeof(char*) * (i + 1));
	if (!cmd_tmp->str)
		return (perror("Error initializing str in cmd_to_node\n"), 1);
	cmd_tok = cmd_tmp->cmds;
	i = 0;
	while (cmd_tok)
	{
		cmd_tmp->str[i] = ft_strdup(cmd_tok->content);//Machter ned
		if (!cmd_tmp->str[i])
			printf("FIX, cmd_to_node\n");
		i++;
		cmd_tok = cmd_tok->next;
	}
	cmd_tmp->str[i] = NULL;
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
	tmp->outfile = NULL;
	tmp->type = token->type;
	return (EXIT_SUCCESS);
}

int	red_to_node(t_token *token, t_cmd_table *node)
{
	t_list		*red_node;
	t_io_red	*tmp;

	red_node = ft_lstnew(malloc(sizeof(t_io_red)));
	if (!red_node || !red_node->content)
	{
		if (red_node)
			free(red_node);
		return (ft_putstr_fd("Error initializing t_list in red_to_node\n", 1), 1);
	}
	tmp = red_node->content;
	if(token->type == REDIR_IN || token->type == HEREDOC)
		input_to_node(token, tmp, node);
	else
	{
		tmp->type = token->type;
		tmp->outfile = token->data;
		tmp->infile = NULL;
	}
	ft_lstadd_back(&node->io_red, red_node);
	return (EXIT_SUCCESS);
}

void	init_cmd_table(t_cmd_table *node)
{
	node->read_fd = 0;
	node->write_fd = 1;
	node->id = -1;
	node->cmds = NULL;
	node->io_red = NULL;
	node->heredoc_name = NULL;
	node->str =	NULL;
	node->exec_path = NULL;
}

void	cmdtok_to_node(t_token *tok, t_cmd_table *cmd)
{
	t_list	*cmd_list;
	t_token	*tmp;

	tmp = tok;
	while (tmp)
	{
		if (tmp->type == 0)
		{
			cmd_list = ft_lstnew(tmp->data);
			if (!cmd_list)
			{
				ft_printerrno("cmdtok_to_node: ");
				return ;
			}
			ft_lstadd_back(&cmd->cmds, cmd_list);
		}
		tmp = tmp->next;
	}
}

void	token_to_struct(t_token *token, t_cmd_table *cmd_node)
{
	t_token	*tmp;

	tmp = token;
	init_cmd_table(cmd_node);
	cmdtok_to_node(tmp, cmd_node);
//	cmd_to_node(tmp, cmd_node);
	while (tmp)
	{
		if (tmp->type >= 1 && tmp->type <= 4)
			red_to_node(tmp, cmd_node);
		tmp = tmp->next;
	}
}

int	tokenize(t_common *c)
{
	t_list	*cmd_tok;
	char	**arr;
	char	**sub_arr;
	int		i;
	int		size;

	size = count_pipes(c->raw_prompt);
	arr = tokenize_one(c->raw_prompt, size + 1);
	i = 0;
    while (arr[i])
    {
        sub_arr = prep_input(arr[i++]);
		cmd_tok = ft_lstnew(malloc(sizeof(t_token)));
		if (!cmd_tok || !cmd_tok->content)
		{
			if (cmd_tok)
				free(cmd_tok);
			return (EXIT_FAILURE);
		}
		add_to_list(sub_arr, cmd_tok);
		ft_lstadd_back(&c->tokens, cmd_tok);
		free_2d(sub_arr);
	}
	free_2d(arr);
	return (EXIT_SUCCESS);
}

int	t_lst_to_struct(t_common *c)
{
	t_list		*cmd_tok;
	t_list_d	*tmp_cmd;

	tmp_cmd = ft_lstnew_d(malloc(sizeof(t_cmd_table)));
	if (!tmp_cmd || !tmp_cmd->content)
	{
		if (tmp_cmd)
			free(tmp_cmd);
		return (perror("Error initializing cmd_struct\n"), 1);
	}
	cmd_tok = c->tokens;
	while (cmd_tok)
	{
		token_to_struct(cmd_tok->content, tmp_cmd->content);
		ft_lst_d_add_back(&c->cmd_struct, tmp_cmd);
		if (cmd_tok->next)
		{
			tmp_cmd = ft_lstnew_d(malloc(sizeof(t_cmd_table)));
			if (!tmp_cmd || !tmp_cmd->content)
			{
				if (tmp_cmd)
					free(tmp_cmd);
				return (perror("Error initializing cmd_struct\n"), 1);
			}
		}
		cmd_tok = cmd_tok->next;
	}
/* 	t_cmd_table *test;
	for (t_list_d *p = c->cmd_struct; p; p = p->next)
	{
		int i = 0;
		test = p->content;
		if (test && test->str){
			while (test->str[i])
			{
				printf("cmd_str->str[%d]: %s\n", i, test->str[i]);
				i++;
			}
		}
	} */
	return (EXIT_SUCCESS);
}

void	ft_cmd_args_to_2d(t_list_d *cmd_table)
{
	t_list_d	*tmp;

	tmp = cmd_table;
	while (tmp)
	{
		cmd_to_node(tmp->content);
		tmp = tmp->next;
	}
}

int	ft_parsing(t_common *c)
{
	if (tokenize(c) == EXIT_FAILURE)
		printf("Tokenize error\n");
	if (t_lst_to_struct(c))
		printf("Token_to_struct error\n");
	ft_expansion(c->env, c->cmd_struct);
	ft_rm_quotes(c->cmd_struct);
	ft_cmd_args_to_2d(c->cmd_struct);
//	t_cmd_table	*cmd;
//	cmd = c->cmd_struct->content;
//	for (int i = 0; cmd->str[i]; i++)
//		printf("%s\n", cmd->str[i]);
	return (EXIT_SUCCESS);
}
