/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_table.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:12:18 by caigner           #+#    #+#             */
/*   Updated: 2024/03/03 23:14:08 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>

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
	int			i;
	t_token		*tmp;
	t_cmd_table *cmd_tmp;

	cmd_tmp = cmd_node;
	tmp = token;
	i = 0;
	while (tmp)
	{
		if (tmp->type == VOID)
			i++;
		tmp = tmp->next;
	}
	cmd_tmp->str = malloc(sizeof(char*) * (i + 1));
	if (!cmd_tmp->str)
		return (perror("Error initializing str in cmd_to_node\n"), 1);
	i = 0;
	while (token)
	{
		if (token->type == VOID)
			cmd_tmp->str[i++] = token->data;
		token = token->next;
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
	cmd_to_node(tmp, cmd_node);
	while (tmp)
	{
		if (tmp->type == REDIR_IN || tmp->type == REDIR_OUT
				|| tmp->type == APPEND || tmp->type == HEREDOC)
			red_to_node(tmp, cmd_node);
		tmp = tmp->next;
	}
}

char	**ft_get_paths(t_env *env)
{
	t_env	*tmp;
	char	**paths;

	tmp = env;
	while (tmp && ft_strncmp(tmp->variable, "PATH", 5))
		tmp = tmp->next;
	paths = ft_split(tmp->value, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

char	*join_path(char *cmd, char *path)
{
	char	*prepath;
	char	*fullpath;

	if (cmd[0] == '/')
		return (NULL);
	prepath = ft_strjoin(path, "/");
	if (!prepath)
		return (NULL);
	fullpath = ft_strjoin(prepath, cmd);
	free(prepath);
	if (!fullpath)
		return (NULL);
	return (fullpath);
}

int	is_dir(char *file)
{
	struct stat	s;
	
	if (stat(file, &s) == 0)
	{
		if (S_ISDIR(s.st_mode))
			return (1);
	}
	return (0);
}

int	add_path(t_cmd_table *cmd, char **paths)
{
	char	*path;
	int		i;
	
	i = 0;
	if (!access(cmd->str[0], F_OK | X_OK | R_OK))
		return (cmd->exec_path = cmd->str[0], EXIT_SUCCESS);
	else
	{
		while (paths[i])
		{
			path = join_path(cmd->str[0], paths[i++]);
			if (!path)
				return (EXIT_FAILURE);
			if (!access(path, F_OK | X_OK | R_OK) && !is_dir(path))
				return (cmd->exec_path = path, EXIT_SUCCESS);
			free(path);
		}
	}
	return (EXIT_FAILURE);
}

void	create_paths(t_common *c, char **paths)
{
	t_list_d	*tmp;

	tmp = c->cmd_struct;
	while (tmp)
	{
		add_path(tmp->content, paths);
		tmp = tmp->next;
	}
}

int	get_cmd_paths(t_common *c)
{
	char	**paths;

	paths = ft_get_paths(c->env);
	if (!paths)
		return (EXIT_FAILURE);
	create_paths(c, paths);
	free_2d(paths);
	return (EXIT_SUCCESS);
}

int	ft_parsing(t_common *c)
{
	t_list		*tmp_tok;
	t_list_d	*tmp_cmd;
	char		**arr;
	char		**sub_arr;
	int			i;
	int			size;

	size = count_pipes(c->raw_prompt);
	arr = tokenize_one(c->raw_prompt, size + 1);
	i = 0;
    while (arr[i])
    {
        sub_arr = prep_input(arr[i++]);
		tmp_tok = ft_lstnew(NULL);
		if (!tmp_tok)
			return (EXIT_FAILURE);
		add_to_list(sub_arr, tmp_tok);
		ft_lstadd_back(&c->tokens, tmp_tok);
		free_2d(sub_arr);
		//free sub_arr, therefore strdup each str?
	}
	tmp_tok = c->tokens;
	tmp_cmd = ft_lstnew_d(malloc(sizeof(t_cmd_table *)));
	if (!tmp_cmd || !tmp_cmd->content)
		return (perror("Error initializing cmd_struct\n"), 1);
	while (tmp_tok)
	{
		token_to_struct(tmp_tok->content, tmp_cmd->content);
		ft_lst_d_add_back(&c->cmd_struct, tmp_cmd);
		if (tmp_tok->next)
		{
			tmp_cmd = ft_lstnew_d(malloc(sizeof(t_cmd_table *)));
			if (!tmp_cmd || !tmp_cmd->content)
				return (perror("Error initializing cmd_struct\n"), 1);
		}
		tmp_tok = tmp_tok->next;
	}
	//ft_expansion(c->cmd_struct);
	//rm_quotes(c->cmd_struct);
	get_cmd_paths(c);
	return (EXIT_SUCCESS);
}
