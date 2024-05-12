/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:35:51 by miheider          #+#    #+#             */
/*   Updated: 2024/05/13 01:17:15 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

/*this function checks for special characters, except for pipes*/
int	check_char(char *character)
{
	char	*special;
	int		i;

	i = 0;
	special = "()<>";
	if (*character == '\'' || *character == '\"')
		return (2);
	while (i < 5)
	{
		if (special[i] == *character)
			return (1);
		i++;
	}
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
char	*handle_quote(char *str, int *in_quotes, char *quote_type)
{
	if (*str == '\"' && *quote_type != '\'')
	{
		*in_quotes = !(*in_quotes);
		if (!(*in_quotes))
			*quote_type = '\0';
		else
			*quote_type = '\"';
	}
	return (str);
}

char	*handle_single_quote(char *str, int *in_quotes, char *quote_type)
{
	if (*str == '\'' && *quote_type != '\"')
	{
		*in_quotes = !(*in_quotes);
		if (!(*in_quotes))
			*quote_type = '\0';
		else
			*quote_type = '\'';
	}
	return (str);
}
