/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:20:30 by caigner           #+#    #+#             */
/*   Updated: 2024/05/13 01:29:45 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	input_to_node(t_common *c, t_token *token, t_io_red *tmp, t_cmd_table *node)
{
	char	*ch;

	if (token->type == HEREDOC)
	{
		tmp->heredoc_limiter = ft_protect(c, ft_strdup(token->data), 0, 0);
		if (node->hd_name)
		{
			free(node->hd_name);
			node->hd_name = NULL;
		}
		ch = ft_itoa(c->heredoc_counter++);
		node->hd_name = ft_protect(c, ft_strjoin(".heredoc_tmp", ch), ch, 0);
		free(ch);
		tmp->infile = ft_protect(c, ft_strdup(node->hd_name), 0, 0);
	}
	else
		tmp->infile = ft_protect(c, ft_strdup(token->data), 0, 0);
	tmp->type = token->type;
	return (EXIT_SUCCESS);
}

void	init_io(t_io_red *io)
{
	io->heredoc_limiter = NULL;
	io->expand_heredoc = 1;
	io->infile = NULL;
	io->outfile = NULL;
}

int	red_to_node(t_common *c, t_token *token, t_cmd_table *node)
{
	t_list		*red_node;
	t_io_red	*tmp;

	tmp = ft_protect(c, malloc(sizeof(t_io_red)), 0, 0);
	red_node = ft_protect(c, ft_lstnew(tmp), tmp, 0);
	init_io(red_node->content);
	tmp = red_node->content;
	if (token->type == REDIR_IN || token->type == HEREDOC)
		input_to_node(c, token, tmp, node);
	else
	{
		tmp->type = token->type;
		tmp->outfile = ft_protect(c, ft_strdup(token->data), tmp, red_node);
	}
	ft_lstadd_back(&node->io_red, red_node);
	return (EXIT_SUCCESS);
}
