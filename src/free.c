/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 02:56:18 by caigner           #+#    #+#             */
/*   Updated: 2024/03/03 17:01:13 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_2d(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
}

void	free_env_nodes(t_env *start)
{
	t_env	*tmp;

	while (start)
	{
		tmp = start;
		start = start->next;
		if (tmp->variable)
			free(tmp->variable);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

void	free_tokens(void *content)
{
	t_token	*token;
	t_token	*tmp;
	
	tmp = (t_token *)content;
	while (tmp)
	{
		token = tmp;
		tmp = token->next;
		free(token->data);
		free (token);
	}
}

void	free_io_red(void *content)
{
	t_io_red	*io;
	
	io = (t_io_red *)content;
	if (io->heredoc_limiter)
		free(io->heredoc_limiter);
	if (io->infile)
		free(io->infile);
	if (io->outfile)
		free(io->outfile);
}

void	free_cmd_table(void *content)
{
	t_cmd_table *table;
	
	table = (t_cmd_table *)content;
	if (table->str)
		free_2d(table->str);
	if (table->exec_path)
		free(table->exec_path);
	if (table->io_red)
		ft_lstclear(&table->io_red, &free_io_red);
	if (table->heredoc_name)
	{
		unlink(table->heredoc_name);
		free(table->heredoc_name);
	}
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
}

void	free_all(t_common *c)
{
//	t_env	*node;
	//rl_clear_history();
	if (c)
	{
		free_env_nodes(c->env);
		free(c);
	}
}
