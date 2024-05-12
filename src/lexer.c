/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 15:59:40 by miheider          #+#    #+#             */
/*   Updated: 2024/05/12 22:00:18 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_in_quotes(char *str, int i)
{
	int		inside_quotes;
	char	current_quote;

	inside_quotes = 0;
	current_quote = '\0';
	while (i >= 0)
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (current_quote == str[i])
			{
				inside_quotes = !inside_quotes;
				current_quote = '\0';
			}
			else if (current_quote == '\0')
			{
				inside_quotes = !inside_quotes;
				current_quote = str[i];
			}
		}
		i--;
	}
	return (inside_quotes);
}

int	ignore_pipe(char *str, int i)
{
	if (i == 0)
		return (0);
	if (is_in_quotes(str, i))
		return (1);
	return (0);
}

/*count "|"-sections within the input. used for allocating memory*/
void	count_pipes_cont(t_common *c, char *input, int *i, int *pipe)
{
	while (input[*i])
	{
		if (input[*i] == '\"' || input[*i] == '\'')
		{
			(*i)++;
			while (input[*i] != '\"' && input[*i] == '\'')
				(*i)++;
		}
		if (input[*i] == '|' && !ignore_pipe(input, *i))
			error_check_pipes(c, i, pipe, input);
		if (input[*i] != '\0')
			(*i)++;
	}
}

int	count_pipes(t_common *c, char *input)
{
	int	i;
	int	pipe;

	i = 0;
	pipe = 0;
	if (input[ft_strlen(input) - 1] == '|')
	{
		if (error_lexer(c, "|", 3))
			return (1);
	}
	while (input[i] && ft_strchr(WHITESPACE, input[i]))
		i++;
	if (input[i] == '|' && !ignore_pipe(input, i))
	{
		if (error_lexer(c, "|", 3))
			return (1);
	}
	count_pipes_cont(c, input, &i, &pipe);
	return (pipe);
}
