/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:42:27 by miheider          #+#    #+#             */
/*   Updated: 2024/05/13 19:44:38 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_add_to_list(int *index, t_token **tmp, t_token **last)
{
	*index = 0;
	*tmp = NULL;
	*last = NULL;
}

void	add_to_list(char **token, t_list *lst)
{
	t_token	*tmp;
	t_token	*last;
	int		index;
	int		status;

	init_add_to_list(&index, &tmp, &last);
	while (token[index])
	{
		status = add_token(lst->content, token, index, &tmp);
		if (status == 1)
			index += 2;
		else if (status <= 0)
			index++;
		if (last == NULL)
		{
			if (lst->content)
				free(lst->content);
			lst->content = tmp;
		}
		else
			last->next = tmp;
		last = tmp;
	}
	if (last != NULL)
		last->next = NULL;
}

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
		if (input[i] == '\'' || input[i] == '\"')
		{
			i++;
			while (input[i] && input[i] != '\'' && input[i] != '\"')
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

/*this is the inner while loop of the set_up_array function. The first 
if statement is handling quotes, the second one is handling different 
situations to put in a space to new_string*/
int	process_special_character(int *i, int *j, char *input, char *new_string)
{
	if (ft_strchr("><", input[(*i) - 1]))
		new_string[(*j)++] = ' ';
	new_string[(*j)++] = input[(*i)++];
	while (input[*i] && check_char(&input[*i]) != 2)
		new_string[(*j)++] = input[(*i)++];
	if (input[*i] && check_char(&input[*i]) == 2)
	{
		new_string[(*j)] = input[(*i)];
		if (input[(*i) + 1] != '\0')
		{
			(*i)++;
			(*j)++;
		}
		if (input[*i] && ft_strchr(WHITESPACE, input[*i]))
			return (-10);
	}
	return (10);
}
