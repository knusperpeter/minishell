/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 19:49:04 by caigner           #+#    #+#             */
/*   Updated: 2024/05/13 01:29:45 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	has_expansion(t_common *c, char *str)
{
	char	*curr_dollar;
	int		i;

	i = 0;
	c->open_double_quotes = 0;
	c->open_single_quotes = 0;
	curr_dollar = ft_strchr(str, '$');
	while (str[i] && curr_dollar)
	{
		handle_quote_state(c, str[i]);
		if (str[i] == '$' && !c->open_single_quotes
			&& !ft_strchr(WHITESPACE, str[i + 1]))
		{
			if (curr_dollar == &str[i])
				return (1);
			else
				curr_dollar = ft_strchr(curr_dollar + 1, '$');
		}
		i++;
	}
	return (0);
}

void	ft_expand_cmds(t_common *c, t_cmd_table **table, t_list *curr)
{
	char	*tmp;
	t_list	*before;

	before = NULL;
	while (curr)
	{
		while (curr && has_expansion(c, curr->content))
		{
			tmp = curr->content;
			curr->content = expand_str(c, curr->content);
			free(tmp);
			check_split(c, table, &curr, before);
		}
		before = curr;
		if (curr)
			curr = curr->next;
	}
}

void	expand_name(t_common *c, t_io_red *io)
{
	char		*tmp;

	if (io->type == REDIR_IN)
	{
		while (has_expansion(c, io->infile))
		{
			tmp = ft_protect(c, ft_strdup(io->infile), 0, 0);
			free(io->infile);
			io->infile = expand_str(c, tmp);
			free(tmp);
		}
	}
	else if (io->type == REDIR_OUT || io->type == APPEND)
	{
		while (has_expansion(c, io->outfile))
		{
			tmp = ft_protect(c, ft_strdup(io->outfile), 0, 0);
			free(io->outfile);
			io->outfile = expand_str(c, tmp);
			free(tmp);
		}
	}
}

void	ft_expand_io(t_common *c, t_list *curr)
{
	t_io_red	*io;

	while (curr)
	{
		io = curr->content;
		if (io)
			expand_name(c, io);
		curr = curr->next;
	}
}

void	ft_expansion(t_common *c, t_list_d *cmds)
{
	t_cmd_table	*cmd_struct;
	t_list		*curr;

	while (cmds)
	{
		cmd_struct = cmds->content;
		curr = cmd_struct->cmds;
		ft_expand_cmds(c, &cmd_struct, curr);
		if (cmd_struct->io_red)
		{
			curr = cmd_struct->io_red;
			ft_expand_io(c, curr);
		}
		cmds = cmds->next;
	}
}
