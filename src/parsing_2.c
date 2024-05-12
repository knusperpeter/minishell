/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:53:44 by caigner           #+#    #+#             */
/*   Updated: 2024/05/13 01:29:45 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	tokenize(t_common *c)
{
	t_list	*cmd_tok;
	t_token	*token;
	char	**arr;
	char	**sub_arr;
	int		i;

	arr = tokenize_one(c, c->raw_prompt, count_pipes(c, c->raw_prompt) + 1);
	if (!arr)
		return (EXIT_FAILURE);
	i = 0;
	while (arr[i])
	{
		token = ft_protect(c, malloc(sizeof(t_token)), 0, 0);
		sub_arr = prep_input(arr[i++]);
		cmd_tok = ft_protect(c, ft_lstnew(token), token, 0);
		add_to_list(sub_arr, cmd_tok);
		ft_lstadd_back(&c->tokens, cmd_tok);
		free_2d(sub_arr);
	}
	return (free_2d(arr), EXIT_SUCCESS);
}

int	cmd_to_node(t_common *c, t_cmd_table *cmd_node)
{
	int			i;
	t_list		*cmd_tok;
	t_cmd_table	*cmd_tmp;

	cmd_tmp = cmd_node;
	cmd_tok = cmd_tmp->cmds;
	i = ft_lstsize(cmd_tok);
	cmd_tmp->str = ft_protect(c, malloc(sizeof(char **) * (i + 1)), 0, 0);
	cmd_tok = cmd_tmp->cmds;
	i = 0;
	while (cmd_tok)
	{
		cmd_tmp->str[i] = ft_protect(c, ft_strdup(cmd_tok->content), 0, 0);
		i++;
		cmd_tok = cmd_tok->next;
	}
	cmd_tmp->str[i] = NULL;
	return (EXIT_SUCCESS);
}

void	ft_cmd_args_to_2d(t_common *c, t_list_d *cmd_table)
{
	t_list_d	*tmp;

	tmp = cmd_table;
	while (tmp)
	{
		cmd_to_node(c, tmp->content);
		tmp = tmp->next;
	}
}

void	init_cmd_table(t_cmd_table *node)
{
	node->read_fd = 0;
	node->write_fd = 1;
	node->id = -1;
	node->cmds = NULL;
	node->io_red = NULL;
	node->hd_name = NULL;
	node->str = NULL;
	node->exec_path = NULL;
	node->in = NULL;
	node->out = NULL;
}
