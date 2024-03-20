/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_table.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:08:45 by miheider          #+#    #+#             */
/*   Updated: 2024/03/20 13:42:11 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * Function: setup_env
 * Description: Converts the linked list of environment variables into an array of strings.
 * Parameter: env - The linked list of environment variables.
 * Returns: An array of strings representing the environment variables.
 */
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
/**
 * Function: cmd_to_node
 * Description: Converts the linked list of commands into an array of strings.
 * Parameter: cmd_node - The linked list of commands.
 * Returns: 0 if successful, 1 if an error occurred.
 */
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
	cmd_tmp->str = malloc(sizeof(char**) * (i + 1));
	if (!cmd_tmp->str)
		return (perror("Error initializing str in cmd_to_node\n"), 1);
	cmd_tok = cmd_tmp->cmds;
	i = 0;
	while (cmd_tok)
	{
		cmd_tmp->str[i] = ft_strdup(cmd_tok->content);
		if (!cmd_tmp->str[i])
			printf("FIX, cmd_to_node\n");
		i++;
		cmd_tok = cmd_tok->next;
	}
	cmd_tmp->str[i] = NULL;
	return (EXIT_SUCCESS);
}
/**
 * Function: input_to_node
 * Description: Handles the input redirection and heredoc tokens.
 * Parameters:
 * - token: The current token.
 * - tmp: The IO redirection structure.
 * - node: The command node.
 * Returns: 0 if successful, 1 if an error occurred.
 */
int	input_to_node(t_token *token, t_io_red *tmp, t_cmd_table *node)
{
	static int	i;
	char		*c;

	i = 0;
	if (token->type == HEREDOC)
	{
		tmp->heredoc_limiter = ft_strdup(token->data);
		if (node->heredoc_name)
		{
			free(node->heredoc_name);
			node->heredoc_name = NULL;
		}
		c = ft_itoa(i++);
		node->heredoc_name = ft_strjoin(".heredoc_tmp", c);
		free(c);
		if (!node->heredoc_name)
			return (perror("Error initializing str in input_to_node\n"), 1);
		tmp->infile = ft_strdup(node->heredoc_name); //CHECK IF .heredoc_tmp already EXISTS, IF YES increment i
	}
	else
		tmp->infile = ft_strdup(token->data);
	tmp->type = token->type;
	return (EXIT_SUCCESS);
}
/**
 * Function: init_io
 * Description: Initializes the IO redirection structure.
 * Parameter: io - The IO redirection structure to initialize.
 */
void	init_io(t_io_red *io)
{
	io->heredoc_limiter = NULL;
	io->infile = NULL;
	io->outfile = NULL;
}
/**
 * Function: red_to_node
 * Description: Handles the redirection tokens and adds them to the command node.
 * Parameters:
 * - token: The current token.
 * - node: The command node.
 * Returns: 0 if successful, 1 if an error occurred.
 */
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
	init_io(red_node->content);
	tmp = red_node->content;
	if(token->type == REDIR_IN || token->type == HEREDOC)
		input_to_node(token, tmp, node);
	else
	{
		tmp->type = token->type;
		tmp->outfile = ft_strdup(token->data);
	}
	ft_lstadd_back(&node->io_red, red_node);
	return (EXIT_SUCCESS);
}
/**
 * Function: init_cmd_table
 * Description: Initializes the command table structure.
 * Parameter: node - The command table structure to initialize.
 */
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
/**
 * Function: cmdtok_to_node
 * Description: Adds command tokens to the command node.
 * Parameters:
 * - tok: The current token.
 * - cmd: The command node.
 */
void	cmdtok_to_node(t_token *tok, t_cmd_table *cmd)
{
	t_list	*cmd_list;
	t_token	*tmp;

	tmp = tok;
	while (tmp)
	{
		if (tmp->type == VOID)
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
/**
 * Function: token_to_struct
 * Description: Converts the token linked list into a command table structure.
 * Parameters:
 * - token: The linked list of tokens.
 * - cmd_node: The command table structure.
 */
void	token_to_struct(t_token *token, t_cmd_table *cmd_node)
{
	t_token	*tmp;

	tmp = token;
	init_cmd_table(cmd_node);
	cmdtok_to_node(tmp, cmd_node);
	while (tmp)
	{
		if (tmp->type >= 1 && tmp->type <= 4)
			red_to_node(tmp, cmd_node);
		tmp = tmp->next;
	}
}

/**
 * Function: tokenize
 * Description: Tokenizes the raw input and adds the tokens to a linked list.
 * Parameter: c - The common structure containing the raw input.
 * Returns: 0 if successful, 1 if an error occurred.
 */
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
/**
 * Function: t_lst_to_struct
 * Description: Converts the token linked list into a command table linked list.
 * Parameter: c - The common structure containing the token linked list.
 * Returns: 0 if successful, 1 if an error occurred.
 */
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
/**
 * Function: ft_cmd_args_to_2d
 * Description: Converts the command arguments into a 2D array.
 * Parameter: cmd_table - The linked list of command tables.
 */
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
/**
 * Function: ft_parsing
 * Description: Parses the raw input and converts it into a command table linked list.
 * Parameter: c - The common structure containing the raw input.
 * Returns: 0 if successful, 1 if an error occurred.
 */
int	ft_parsing(t_common *c)
{
	if (tokenize(c) == EXIT_FAILURE)
		printf("Tokenize error\n");
	if (t_lst_to_struct(c))
		printf("Token_to_struct error\n");
//TEST
/* 	t_list_d	*cmd_list = NULL;
	cmd_list = ft_lstnew_d(malloc(sizeof(t_cmd_table)));
	t_cmd_table *test = cmd_list->content;
	test->cmds = ft_lstnew(malloc(sizeof(t_list)));
	t_list		*cmds = test->cmds;
	cmds->content = "\"'$USER\"''";
	ft_expansion(c->env, cmd_list);
	ft_rm_quotes(cmd_list); */
//END
	ft_expansion(c, c->cmd_struct);
	ft_rm_quotes(c->cmd_struct);
	ft_cmd_args_to_2d(c->cmd_struct);
	return (EXIT_SUCCESS);
}
