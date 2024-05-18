/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenizing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 20:31:47 by miheider          #+#    #+#             */
/*   Updated: 2024/05/18 20:32:44 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*counts the spaces of a token, used to allocate memory in function 
tokenize_input*/
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

/*tokenizes the tokens, stores it in the result array and returns it*/
char	**tokenize_input(char *input)
{
	char	**result;
	char	*token;
	int		index;
	int		tok;

	tok = check_tokens(input);
	result = malloc(sizeof(char *) * (tok + 2));
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

/*this function looks for <>, ><, >><< and <<>> in the string and updates
the character counter*/
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

/*this function is used by the prep_input function to count the characters (cc)
and move the index (i).*/
void	counting_up(int *i, int *cc, int a, int b)
{
	(*i) += a;
	(*cc) += b;
}

/*this function is tokenizing the input for the first time. the delimiter are
the '|'. after tokenizing this function checks every token for syntax errors.*/
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