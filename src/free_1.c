/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 02:56:18 by caigner           #+#    #+#             */
/*   Updated: 2024/05/19 13:34:02 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_cmd_lst(t_list **lst)
{
	t_list	*tmp;

	while (lst && *lst)
	{
		tmp = (*lst)->next;
		free((*lst)->content);
		free(*lst);
		*lst = tmp;
	}
}

void	free_cmd_table(void *content)
{
	t_cmd_table	*table;

	table = (t_cmd_table *)content;
	if (table->str)
		free_2d(table->str);
	if (table->exec_path)
		free(table->exec_path);
	if (table->io_red)
		ft_lstclear(&table->io_red, &free_io_red);
	if (table->cmds)
		free_cmd_lst(&table->cmds);
	if (table->hd_name)
	{
		unlink(table->hd_name);
		free(table->hd_name);
		table->hd_name = NULL;
	}
	free(content);
}

void	ft_cleanup_loop(t_common *c)
{
	ft_lst_d_clear(&c->cmd_struct, &free_cmd_table);
	ft_lstclear(&c->tokens, &free_tokens);
	if (c->envp)
	{
		free_2d(c->envp);
		c->envp = NULL;
	}
	free(c->raw_prompt);
	c->raw_prompt = NULL;
}

void	free_all(t_common *c, int cleanup_loop)
{
	if (cleanup_loop)
		ft_cleanup_loop(c);
	if (c->env)
		free_env_nodes(c->env);
	if (c->hidden_env)
		free_env_nodes(c->hidden_env);
	rl_clear_history();
}

void	ft_clean_exit(t_common *c, char *msg, int cleanup_loop)
{
	if (msg)
		printf("%s\n", msg);
	free_all(c, cleanup_loop);
	exit(c->exitstatus);
}
