/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:55:02 by caigner           #+#    #+#             */
/*   Updated: 2024/05/12 17:55:45 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * Function: free_2d
 * Description: Frees a 2D array of strings.
 * Parameter: str - The 2D array to free.
 */
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

/**
 * Function: free_env_nodes
 * Description: Frees a linked list of environment variables.
 * Parameter: start - The start of the linked list to free.
 */
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

/**
 * Function: free_tokens
 * Description: Frees a linked list of tokens.
 * Parameter: content - The linked list to free.
 */
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

/**
 * Function: free_io_red
 * Description: Frees an IO redirection structure.
 * Parameter: content - The IO redirection structure to free.
 */
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