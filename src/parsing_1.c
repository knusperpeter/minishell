/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:08:45 by miheider          #+#    #+#             */
/*   Updated: 2024/05/12 17:46:58 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_cmd_table(t_cmd_table *node)
{
	node->read_fd = 0;
	node->write_fd = 1;
	node->id = -1;
	node->cmds = NULL;
	node->io_red = NULL;
	node->heredoc_name = NULL;
	node->str = NULL;
	node->exec_path = NULL;
	node->in = NULL;
	node->out = NULL;
}

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

t_list_d	*create_cmds_node(t_common *c)
{
	t_list_d	*tmp_cmd;
	t_cmd_table	*cmd_table;

	cmd_table = malloc(sizeof(t_cmd_table));
	if (!cmd_table)
		return (c->exitstatus = 1, ft_clean_exit(c, "malloc-fail", 1), NULL);
	tmp_cmd = ft_lstnew_d(cmd_table);
	if (!tmp_cmd)
		return (c->exitstatus = 1, free(cmd_table),
			ft_clean_exit(c, "malloc-fail", 1), NULL);
	return (tmp_cmd);
}

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
	ft_rm_quotes(c, c->cmd_struct);
	cmd_str = c->cmd_struct;
	while (cmd_str)
	{
		cmd_table = cmd_str->content;
		if (open_io(c, cmd_table->io_red, cmd_table) == 5)
			return (EXIT_FAILURE);
		cmd_str = cmd_str->next;
	}
	cmd_str = c->cmd_struct;
	ft_cmd_args_to_2d(c->cmd_struct);
	return (EXIT_SUCCESS);
}
