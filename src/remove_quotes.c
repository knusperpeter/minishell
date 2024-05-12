/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 11:37:09 by chris             #+#    #+#             */
/*   Updated: 2024/05/12 11:49:16 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_str_wo_quotes(char *str)
{
	char	*value;

	value = NULL;
	value = ft_strdup(str);
	return (value);
}

char	*ft_rm_quotes_str(t_common *c, char *str)
{
	int		i;
	int		j;

	if (!str)
		return (NULL);
	c->open_single_quotes = 0;
	c->open_double_quotes = 0;
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '\'' && !c->open_double_quotes) || (str[i] == '\"'
				&& !c->open_single_quotes))
            handle_quote_state(c, str[i]);
		else
			str[j++] = str[i];
		i++;
	}
	return (str[j] = 0, str);
}

void	ft_rm_quotes_io(t_common *c, t_list *io_lst)
{
	t_io_red	*io;
	int			limiter_len;

	while (io_lst)
	{
		io = io_lst->content;
		if (!io)
			return ;
		io->infile = ft_rm_quotes_str(c, io->infile);
		io->outfile = ft_rm_quotes_str(c, io->outfile);
		if (io->heredoc_limiter)
		{
			limiter_len = ft_strlen(io->heredoc_limiter);
			io->heredoc_limiter = ft_rm_quotes_str(c, io->heredoc_limiter);
			if ((int)ft_strlen(io->heredoc_limiter) != limiter_len)
				io->expand_heredoc = 0;
		}
		io_lst = io_lst->next;
	}
}

void	ft_rm_in_cmd(t_common *c, t_cmd_table *cmd)
{
	t_list	*tmp_cmd;
	t_list	*tmp_io;

	if (!cmd)
		return ;
	tmp_cmd = cmd->cmds;
	while (tmp_cmd)
	{
		tmp_cmd->content = ft_rm_quotes_str(c, tmp_cmd->content);
		tmp_cmd = tmp_cmd->next;
	}
	tmp_io = cmd->io_red;
	ft_rm_quotes_io(c, tmp_io);
}

void	ft_rm_quotes(t_common *c, t_list_d *cmds)
{
	t_list_d	*tmp;

	tmp = cmds;
	while (tmp)
	{
		ft_rm_in_cmd(c, tmp->content);
		tmp = tmp->next;
	}
}
