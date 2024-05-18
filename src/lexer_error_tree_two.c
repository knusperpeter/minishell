/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error_tree_two.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 20:15:38 by miheider          #+#    #+#             */
/*   Updated: 2024/05/18 20:18:20 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*This function returns an error message if it finds an empty single or 
double quote or quotes which contains only whitespaces.*/
int	check_empty_quotes(t_common *c, char *input)
{
	int		i;
	char	quote;
	int		k;

	quote = '\0';
	k = 0;
	i = 0;
	i = skip_whitespace(input, i, 0);
	if (input[i] == '\'' || input[i] == '\"')
	{
		quote = input[i];
		i++;
		k = i;
	}
	k = skip_whitespace(input, k, 0);
	if (input[k] == quote)
	{
		while (i < k)
			write (2, &input[i++], 1);
		ft_putstr_fd(MESSAGE13, 2);
		return (es_cul(c, 127), 1);
	}
	return (0);
}

/*This is a function which checks for open double or single quotes. If there are
open quotes it will clear and return the prompt (exitstatus 2).*/
int	check_open_quotes(t_common *c, char *result)
{
	int	i;
	int	s_quote;
	int	d_quote;

	i = 0;
	s_quote = 0;
	d_quote = 0;
	c->open_single_quotes = 0;
	c->open_double_quotes = 0;
	while (result[i])
	{
		handle_quote_state(c, result[i]);
		if (result[i] == '\"' && c->open_single_quotes == 0)
			d_quote++;
		else if (result[i] == '\'' && c->open_double_quotes == 0)
			s_quote++;
		i++;
	}
	if (c->open_single_quotes == 1 || c->open_double_quotes == 1)
	{
		ft_putstr_fd(MESSAGE12, 2);
		return (es_cul(c, 2), 1);
	}
	return (0);
}

int	check_next(char *input, int i)
{
	int	len;

	len = ft_strlen(input);
	if (i + 1 <= len)
	{
		if (i + 2 <= len && input[i] == '<' && input[i + 1] == '<')
			return (i + 2);
		else if (i + 2 <= len && input[i] == '>' && input[i + 1] == '>')
			return (i + 2);
		else
			return (i + 1);
	}
	return (i + 1);
}

int	check_invalid_redir(t_common *c, char *input)
{
	size_t	i;
	int		status;

	status = 0;
	i = 0;
	i = skip_whitespace(input, i, 0);
	if (i + 1 <= ft_strlen(input) && (input[i] == '<' || input[i] == '>'))
	{
		i = check_next(input, i);
		if (input[i] != ' ')
			return (0);
		i = skip_whitespace(input, i, 0);
		if (input[i] == '>' && input[i + 1] == '>')
			status = ft_putstr_fd(MESSAGE4, 2);
		else if (input[i] == '<' && input[i + 1] == '<')
			status = ft_putstr_fd(MESSAGE3, 2);
		else if (input[i] == '<')
			status = ft_putstr_fd(MESSAGE5, 2);
		else if (input[i] == '>')
			status = ft_putstr_fd(MESSAGE6, 2);
	}
	if (status)
		return (es_cul(c, 2), 1);
	return (0);
}

/*This function calls many function which will check for different syntax errors
in every token. if an error occours it will catch the return value and returns 1
itself.*/
int	error_tree(t_common *c, char *input)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	if (check_open_quotes(c, input))
		return (1);
	if (check_empty_quotes(c, input))
		return (1);
	if (check_dots(c, input, i, k))
		return (1);
	if (check_invalid_redir(c, input))
		return (1);
	if (check_block_redir(c, input))
		return (1);
	if (check_last_redir(c, input))
		return (1);
	return (0);
}