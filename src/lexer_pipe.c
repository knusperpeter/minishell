/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 20:07:01 by miheider          #+#    #+#             */
/*   Updated: 2024/05/19 13:09:26 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_empty_line(t_common *c, char *input)
{
	int	i;

	i = 0;
	i = skip_whitespace(input, i, 0);
	if (input[i] == '\0')
		return (ft_cleanup_loop(c), 1);
	return (0);
}

/*if the last printable character of the input is a pipe ('|') it enters the 
error_lexer function and displays an error message (incl freeing)*/
int	check_pipe_error_last(t_common *c, char *input, int len)
{
	len = skip_whitespace(input, len - 1, 1);
	if (input[len] == '|')
		return (error_lexer(c, 1), 1);
	return (0);
}

int	handle_i_and_pipe(char *input, int i, int *pipe)
{
	i++;
	i = skip_whitespace(input, i, 0);
	*pipe = 1;
	return (i);
}

/*this function checks if an pipe ('|') related syntax error occurs 
and enters the error_lexer function and displays an error message 
(incl freeing) if this is the case*/
int	check_pipe_error(t_common *c, char *input, int len)
{
	int	i;
	int	pipe;

	i = 0;
	pipe = 0;
	i = skip_whitespace(input, i, 0);
	if (i + 1 < len && input[i] == '|'
		&& input[i + 1] == '|' && !q_status(input, i))
		return (error_lexer(c, 2), -1);
	if (i < len && input[i] == '|' && !q_status(input, i))
		return (error_lexer(c, 1), -1);
	while (input[i])
	{
		if (input[i] == '|' && !q_status(input, i))
			i = handle_i_and_pipe(input, i, &pipe);
		if (input[i] == '|' && input[i - 1] == '|' && !q_status(input, i))
			return (error_lexer(c, 2), -1);
		if (input[i] == '|' && !q_status(input, i) && pipe == 1)
			return (error_lexer(c, 1), -1);
		pipe = 0;
		if (input[i] != '\0')
			i++;
	}
	return (0);
}

/*this function counts all the pipes ('|') which are not in quotes. it 
checks for pipe ('|') related errors and returns -1 in case of an error*/
int	count_pipes(t_common *c, char *input)
{
	int	i;
	int	pipe;
	int	len;
	int	in_quote;

	i = 0;
	pipe = 0;
	in_quote = 0;
	len = ft_strlen(input);
	if (check_open_quotes(c, input) != 0
		|| check_pipe_error(c, input, len) != 0
		|| check_pipe_error_last(c, input, len) != 0
		|| check_empty_line(c, input) != 0)
		return (-1);
	while (input[i])
	{
		in_quote = q_status(&input[0], i);
		if (input[i] == '|' && in_quote == 0)
			pipe++;
		i++;
	}
	return (pipe);
}
