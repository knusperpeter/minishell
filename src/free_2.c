/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:55:02 by caigner           #+#    #+#             */
/*   Updated: 2024/05/19 13:34:25 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_2d(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
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

	if (!content)
		return ;
	tmp = (t_token *)content;
	while (tmp)
	{
		token = tmp;
		tmp = token->next;
		if (token->data)
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
	free(io);
}
