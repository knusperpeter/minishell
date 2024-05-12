/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:20:30 by caigner           #+#    #+#             */
/*   Updated: 2024/05/12 17:29:20 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		tmp->infile = ft_strdup(node->heredoc_name);
		//CHECK IF .heredoc_tmp already EXISTS, IF YES increment i
	}
	else
		tmp->infile = ft_strdup(token->data);
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

	tmp = malloc(sizeof(t_io_red));
	//protect
	red_node = ft_lstnew(tmp);
	if (!red_node)
		return (ft_putstr_fd("Error in red_to_node\n", 1), 1);
	init_io(red_node->content);
	tmp = red_node->content;
	if (token->type == REDIR_IN || token->type == HEREDOC)
		input_to_node(c, token, tmp, node);
	else
	{
		tmp->type = token->type;
		tmp->outfile = ft_strdup(token->data);
	}
	ft_lstadd_back(&node->io_red, red_node);
	return (EXIT_SUCCESS);
}
