/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:08:45 by miheider          #+#    #+#             */
/*   Updated: 2024/05/11 11:51:23 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
int	input_to_node(t_common *c, t_token *token, t_io_red *tmp, t_cmd_table *node)
{
	char	*ch;

	if (token->type == HEREDOC)
	{
		tmp->heredoc_limiter = ft_strdup(token->data);
		if (node->heredoc_name)
		{
			free(node->heredoc_name);
			node->heredoc_name = NULL;
		}
		ch = ft_itoa(c->heredoc_counter++);
		node->heredoc_name = ft_strjoin(".heredoc_tmp", ch);
		free(ch);
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
	io->expand_heredoc = 1;
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
int	red_to_node(t_common *c, t_token *token, t_cmd_table *node)
{
	t_list		*red_node;
	t_io_red	*tmp;

	tmp = malloc(sizeof(t_io_red));
	//protect
	red_node = ft_lstnew(tmp);
	if (!red_node)
		return (ft_putstr_fd("Error initializing t_list in red_to_node\n", 1), 1);
	init_io(red_node->content);
	tmp = red_node->content;
	if(token->type == REDIR_IN || token->type == HEREDOC)
		input_to_node(c, token, tmp, node);
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
	node->infile = NULL;
	node->outfile = NULL;
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
	char	*tmp_cmd;

	tmp = tok;
	while (tmp)
	{
		if (tmp->type == VOID)
		{
			tmp_cmd = ft_strdup(tmp->data);
			//protect
			cmd_list = ft_lstnew(tmp_cmd);
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
void	token_to_struct(t_common *c, t_token *token, t_cmd_table *cmd_node)
{
	t_token	*tmp;

	tmp = token;
	init_cmd_table(cmd_node);
	cmdtok_to_node(tmp, cmd_node);
	tmp = token;
	while (tmp)
	{
		if (tmp->type >= 1 && tmp->type <= 4)
			red_to_node(c, tmp, cmd_node);
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
	t_token	*token;
	char	**arr;
	char	**sub_arr;
	int		i;
	int		size;

	size = count_pipes(c, c->raw_prompt);
	arr = tokenize_one(c, c->raw_prompt, size + 1);
	if(!arr)
		return (EXIT_FAILURE);
	i = 0;
	while (arr[i])
	{
		sub_arr = prep_input(arr[i++]);
		token = malloc(sizeof(t_token));
		if (!token)
			return (c->exitstatus = 1, ft_clean_exit(c, "malloc-fail", 0), EXIT_FAILURE);
		cmd_tok = ft_lstnew(token);
		if (!cmd_tok)
			return (c->exitstatus = 1, free(token), ft_clean_exit(c, "malloc-fail", 0), EXIT_FAILURE);
		add_to_list(sub_arr, cmd_tok);
		ft_lstadd_back(&c->tokens, cmd_tok);
		free_2d(sub_arr);
	}
	free_2d(arr);
	return (EXIT_SUCCESS);
}

t_list_d	*create_cmds_node(t_common *c)
{
	t_list_d	*tmp_cmd;
	t_cmd_table	*cmd_table;

	cmd_table = malloc(sizeof(t_cmd_table));
	if (!cmd_table)
		return (c->exitstatus = 1, ft_clean_exit(c, "malloc-fail", 1), NULL);
	tmp_cmd = ft_lstnew_d(cmd_table);
	if (!tmp_cmd)
		return (c->exitstatus = 1, free(cmd_table), ft_clean_exit(c, "malloc-fail", 1), NULL);
	return (tmp_cmd);
}

/**
 * Function: t_lst_to_struct
 * Description: Converts the token linked list into a command table linked list.
 * Parameter: c - The common structure containing the token linked list.
 * Returns: 0 if successful, 1 if an error occurred.
 */
int	t_lst_to_struct(t_common *c)
{
	t_list		*cmd_token;
	t_list_d	*tmp_cmd;

	cmd_token = c->tokens;
	while (cmd_token)
	{
		tmp_cmd = create_cmds_node(c);
		token_to_struct(c, cmd_token->content, tmp_cmd->content);
		ft_lst_d_add_back(&c->cmd_struct, tmp_cmd);
		cmd_token = cmd_token->next;
	}
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
	t_list_d	*cmd_str;
	t_cmd_table	*cmd_table;

	tokenize(c);
	if (t_lst_to_struct(c))
		printf("Token_to_struct error\n");
	cmd_str = c->cmd_struct;
	ft_expansion(c, cmd_str);
	cmd_str = c->cmd_struct;
	ft_rm_quotes(c->cmd_struct);
	//io_open should be here instead of in t_lst_to_struct!!!!!
	cmd_str = c->cmd_struct;
	while (cmd_str)
	{
		cmd_table = cmd_str->content;
		open_io(c, cmd_table->io_red, cmd_table);
		cmd_str = cmd_str->next;
	}
	cmd_str = c->cmd_struct;
	ft_cmd_args_to_2d(c->cmd_struct);
	return (EXIT_SUCCESS);
}
