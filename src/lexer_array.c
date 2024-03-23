/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 14:56:52 by miheider          #+#    #+#             */
/*   Updated: 2024/03/23 15:17:43 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*tokenizes the tokens, stores it in the result array and returns it*/
char	**tokenize_input(char *input)
{
	char	**result;
	char	*token;
	int		index;
	int		tok;

	tok = check_token(input);
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
	return (result);
}

/*this is the inner while loop of the set_up_array function. The first 
if statement is handling quotes, the second one is handling different 
situations to inpute a space to new_string*/
int	no_space_array(int *i, int *j, char *input, char *new_string)
{
	if (input[*i] && check_char(&input[*i]) == 2)
	{
		new_string[(*j)++] = input[(*i)++];
		while (input[*i] && check_char(&input[*i]) != 2)
			new_string[(*j)++] = input[(*i)++];
		if (input[*i] && check_char(&input[*i]) == 2)
		{
			new_string[(*j)++] = input[(*i)++];
			if (input[*i] && strchr(WHITESPACE, input[*i]))
				return (-10);
		}
	}
	if (*i > 0)
	{
		if ((input[*i] && check_char(&input[*i]) == 0
				&& check_char(&input[*(i) - 1]) == 1)
			|| (check_char(&input[*i]) == 1
				&& check_char(&input[*(i) - 1]) == 0
				&& !strchr(WHITESPACE, input[(*i) - 1]))
			|| (check_char(&input[*i]) == 1
				&& check_char(&input[*(i) - 1]) == 1
				&& input[*i] != input[*(i) - 1]))
		{
			new_string[(*j)++] = ' ';
		}
	}
	new_string[(*j)++] = input[(*i)++];
	return (10);
}

/*just allocating memory for set_up_array fuction*/
char	*allocate_memory(int size)
{
	char	*new_string;

	new_string = (char *)malloc(sizeof(char) * (size + 1));
	if (!new_string)
	{
		printf("Memory allocation failed!\n");
		exit(1);
	}
	return (new_string);
}

/*this function is preparing the tokenized input line by line */
char	**set_up_array(int cc, char *input)
{
	char	*new_string;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_string = allocate_memory(cc);
	while (input[i] && strchr(WHITESPACE, input[i]))
		i++;
	while (input[i])
	{
		while (input[i] && !strchr(WHITESPACE, input[i]))
		{
			if (no_space_array(&i, &j, input, new_string) < 0)
				break ;
		}
		while (input[i] && strchr(WHITESPACE, input[i]))
			i++;
		if (i > 0)
			if (input[i] != '\0' && strchr(WHITESPACE, input[i - 1]))
				new_string[j++] = ' ';
	}
	new_string[j] = '\0';
	return (tokenize_input(new_string));
}
