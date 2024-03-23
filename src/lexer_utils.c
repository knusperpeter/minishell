/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 16:23:20 by miheider          #+#    #+#             */
/*   Updated: 2024/03/23 15:21:16 by miheider         ###   ########.fr       */
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

/*this function checks for special characters, except for pipes*/
int	check_char(char *character)
{
	char	*special;
	int		i;

	i = 0;
	special = "*[]()<>#";
	if (*character == '\'' || *character == '\"')
		return (2);
	while (i < 9)
	{
		if (special[i] == *character)
			return (1);
		i++;
	}
	return (0);
}

/*This functions handles the quotes section for the ft_strtok function*/
char	*quotes_in_strtok(char *str, const char *delim)
{
	int	in_quotes;

	in_quotes = 0;
	while (*str)
	{
		if (*str == '\"' || *str == '\'')
			in_quotes = !in_quotes;
		if (!in_quotes && ft_strchr(delim, *str))
			break ;
		if (!in_quotes && *str == '\0')
			error_lexer("\"", 5);
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

	if (s1)
		str = s1;
	if (!str)
		return (0);
	while (*str && ft_strchr(delim, *str))
		str++;
	if (*str == '\0')
		return (0);
	start = str;
	str = quotes_in_strtok(str, delim);
	if (*str != '\0')
	{
		*str = '\0';
		str++;
	}
	return (start);
}
