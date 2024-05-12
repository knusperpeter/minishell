/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_9.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:58:30 by miheider          #+#    #+#             */
/*   Updated: 2024/05/13 01:19:29 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	initial_checks(t_common *c, char *result)
{
	int		i;
	int		k;
	char	fir;
	int		len;

	init_initial_checks(&i, &k, &len, result);
	if (check_sq(c, &result[k], k) || open_quotes(c, &result[0])
		|| check_one_more(c, &result[0]))
		return (1);
	while (len >= 1 && (result[i] == ' '
			|| (result[i] >= 9 && result[i] <= 13)))
		i++;
	if (len >= 1 && (result[i] == '<' || result[i] == '>'))
		fir = result[i];
	else if (result[i] == '.')
		fir = result[i];
	else if (result[i] == '.' && (result[i + 1] == '\0'
			|| (len >= 2 && result[i + 1] == '.')))
		fir = result[i];
	else if (result[i] == '\"')
		fir = result[i];
	else
		return (0);
	k = i;
	return (check_following(c, result, k, fir));
}

int	check_result(t_common *c, char *result)
{
	return (initial_checks(c, result));
}

int	check_empty_line(t_common *c, char *input, int pipe)
{
	int	i;

	i = 0;
	if (pipe > 1)
		return (0);
	while (input[i])
	{
		if (input[i] == ' ' || (input[i] > 9 && input[i] < 13))
			i++;
		else
			return (0);
	}
	c->exitstatus = 131;
	return (1);
}

/*tokenize the input the first time using the "|" as an delimiter*/
char	**tokenize_one(t_common *c, char *input, int pipe)
{
	char	**result;
	char	*token;
	int		index;

	result = malloc((pipe + 1) * sizeof(char *));
	if (result == NULL)
	{
		printf("Error - malloc - tokenize_one\n");
		return (NULL);
	}
	token = ft_strtok(input, "|");
	index = 0;
	while (token != NULL && index < pipe)
	{
		result[index] = ft_strdup(token);
		if (check_empty_line(c, input, pipe))
			return (c->exitstatus = 0, free(result[index]), free(result), NULL);
		if (check_result(c, result[index]))
			return (free(result[index]), free(result), NULL);
		token = ft_strtok(NULL, "|");
		index++;
	}
	result[index] = NULL;
	return (result);
}

/*This is an extension for the count_pipes function. It handles the pipes 
in case of appearance.*/
int	error_check_pipes(t_common *c, int *i, int *pipe, char *input)
{
	(*i)++;
	if (input[*i] == '\0')
	{
		if (error_lexer(c, "|", 2))
			return (1);
	}
	while (input[*i] && ft_strchr(WHITESPACE, input[*i]))
	{
		if (input[*i] && ft_strchr(WHITESPACE, input[*i])
			&& input[*(i)] == '\0')
		{
			if (error_lexer(c, "|", 2))
				return (1);
		}
		(*i)++;
	}
	if (input[*i] == '|')
	{
		if (error_lexer(c, "|", 2))
			return (1);
	}
	else
		*pipe += 1;
	return (0);
}
