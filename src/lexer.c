/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:36:38 by miheider          #+#    #+#             */
/*   Updated: 2024/05/18 20:48:04 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*checks token for <, >, >>, <<!*/
int	check_token(char *token)
{
	int	len;

	if (!token)
		return (-1);
	len = ft_strlen(token);
	if (token[0] == '<' && len == 1)
		return (1);
	if (token[0] == '>' && len == 1)
		return (2);
	if (token[0] == '>' && token[1] == '>' && len >= 2)
		return (3);
	if (token[0] == '<' && token[1] == '<' && len >= 2)
		return (4);
	return (0);
}

int	is_delim(char c, const char *delim)
{
	while (*delim)
	{
		if (c == *delim)
			return (1);
		delim++;
	}
	return (0);
}

/*This functions handles the quotes section for the ft_strtok function*/
char	*quotes_in_strtok(char *str, const char *delim, int in_quotes,
			char quote_type)
{
	while (*str)
	{
		if (*str == '\"' && quote_type != '\'')
		{
			in_quotes = !in_quotes;
			if (!in_quotes)
				quote_type = '\0';
			else
				quote_type = '\"';
		}
		else if (*str == '\'' && quote_type != '\"')
		{
			in_quotes = !in_quotes;
			if (!in_quotes)
				quote_type = '\0';
			else
				quote_type = '\'';
		}
		if (!in_quotes && is_delim(*str, delim))
			break ;
		str++;
	}
	return (str);
}

/*tokenizing the input; rebuilt the original function strtok and 
addes a feature: it is ignoring the delimiter in quotes.*/
char	*ft_strtok(char *s1, const char *delim)
{
	static char	*str;
	char		*start;
	int			in_quotes;
	char		quote_type;

	in_quotes = 0;
	quote_type = '\0';
	if (s1)
		str = s1;
	if (!str)
		return (0);
	while (*str && ft_strchr(delim, *str))
		str++;
	if (*str == '\0')
		return (0);
	start = str;
	str = quotes_in_strtok(str, delim, in_quotes, quote_type);
	if (*str != '\0')
	{
		*str = '\0';
		str++;
	}
	return (start);
}
