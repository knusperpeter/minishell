/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenizing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 20:31:47 by miheider          #+#    #+#             */
/*   Updated: 2024/05/20 15:40:43 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_tokens(char *input)
{
	int	i;
	int	space;

	i = 0;
	space = 0;
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '\"') && input[i + 1] != '\0')
		{
			i++;
			while (input[i] && input[i] && input[i] != '\'' && input[i] != '\"')
				i++;
		}
		if (input[i] && ft_strchr(WHITESPACE, input[i]))
			space++;
		i++;
	}
	return (space);
}

char	**tokenize_input(t_common *c, char *input)
{
	char	**result;
	char	*token;
	int		index;
	int		tok;

	tok = check_tokens(input);
	result = ft_protect(c, malloc(sizeof(char *) * (tok + 2)), 0, 0);
	if (!result)
	{
		printf("Error - malloc - tokenize_one\n");
		return (NULL);
	}
	token = ft_strtok(input, " ");
	index = 0;
	while (token != NULL)
	{
		result[index++] = ft_strdup(token);
		token = ft_strtok(NULL, " ");
	}
	result[index] = NULL;
	free(input);
	return (result);
}

int	check_double(char *input, int i, int len)
{
	if (i + 1 <= len && input[i] == '<' && input[i + 1] == '>')
		return (1);
	if (i + 1 <= len && input[i] == '>' && input[i + 1] == '<')
		return (1);
	if (i + 3 <= len && input[i] == '<' && input[i + 1] == '<'
		&& input[i + 2] == '>' && input[i + 3] == '>')
		return (2);
	if (i + 3 <= len && input[i] == '>' && input[i + 1] == '>'
		&& input[i + 2] == '<' && input[i + 3] == '<')
		return (2);
	return (0);
}

void	counting_up(int *i, int *cc, int a, int b)
{
	(*i) += a;
	(*cc) += b;
}

char	**tokenize_one(t_common *c, char *input, int pipe)
{
	char	**result;
	char	*token;
	int		index;

	result = ft_protect(c, malloc((pipe + 1) * sizeof(char *)), 0, 0);
	token = ft_strtok(input, "|");
	index = 0;
	while (token != NULL && index < pipe)
	{
		result[index] = ft_protect(c, ft_strdup(token), result, 0);
		if (error_tree(c, result[index]))
			return (free(result[index]), free(result), NULL);
		token = ft_strtok(NULL, "|");
		index++;
	}
	result[index] = NULL;
	return (result);
}
