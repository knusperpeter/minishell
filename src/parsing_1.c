/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:08:45 by miheider          #+#    #+#             */
/*   Updated: 2024/05/13 01:29:45 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	cmdtok_to_node(t_common *c, t_token *tok, t_cmd_table *cmd)
{
	t_list	*cmd_list;
	t_token	*tmp;
	char	*tmp_cmd;

	tmp = tok;
	while (tmp)
	{
		if (tmp->type == VOID)
		{
			tmp_cmd = ft_protect(c, ft_strdup(tmp->data), 0, 0);
			cmd_list = ft_protect(c, ft_lstnew(tmp_cmd), tmp_cmd, 0);
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
	cmdtok_to_node(c, tmp, cmd_node);
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

	cmd_table = ft_protect(c, malloc(sizeof(t_cmd_table)), 0, 0);
	tmp_cmd = ft_protect(c, ft_lstnew_d(cmd_table), cmd_table, 0);
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
		ft_lst_d_add_back(&c->cmd_struct, tmp_cmd);//moved one line up
		token_to_struct(c, cmd_token->content, tmp_cmd->content);
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
	ft_cmd_args_to_2d(c, c->cmd_struct);
	return (EXIT_SUCCESS);
}
