/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:38:16 by miheider          #+#    #+#             */
/*   Updated: 2024/05/13 01:16:32 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*quotes_in_strtok(char *str, const char *delim)
{
	int		in_quotes;
	char	quote_type;

	in_quotes = 0;
	quote_type = '\0';
	while (*str)
	{
		str = handle_quote(str, &in_quotes, &quote_type);
		str = handle_single_quote(str, &in_quotes, &quote_type);
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

int	create_token(t_token **token, char *data)
{
	*token = malloc(sizeof(t_token));
	if (!*token)
		return (-1);
	(*token)->data = ft_strdup(data);
	if (!(*token)->data)
	{
		ft_putstr_fd("malloc token->data error\n", 1);
		free(*token);
		return (-1);
	}
	return (0);
}

int	check_and_create_token(char **value, int i, t_token **token)
{
	int	token_type;

	token_type = check_token(value[i]);
	if (token_type >= 1 && token_type <= 4)
	{
		if (value[i + 1])
		{
			if (create_token(token, value[i + 1]) == -1)
				return (-1);
		}
		else
		{
			ft_putstr_fd("minishell❌: syntax error\n", 1);
			return (-1);
		}
	}
	else
	{
		if (create_token(token, value[i]) == -1)
			return (-1);
	}
	return (token_type);
}

int	add_token(t_token **lst, char **value, int i, t_token **tmp)
{
	t_token	*token;
	int		ret;
	int		token_type;

	*tmp = NULL;
	(void)*lst;
	ret = 0;
	if (!value[i])
	{
		ft_putstr_fd("minishell❌: syntax error\n", 1);
		return (-1);
	}
	token_type = check_and_create_token(value, i, &token);
	if (token_type == -1)
		return (-1);
	token->type = token_type;
	*tmp = token;
	if (token_type >= 1 && token_type <= 4)
		ret = 1;
	else
		ret = 0;
	return (ret);
}
