/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:09:24 by miheider          #+#    #+#             */
/*   Updated: 2024/03/23 16:34:06 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	add_token(t_token **lst, char **value, int i, t_token **tmp)
{
	t_token	*token;
	int		ret;

	(void) lst;
	ret = 0;
	token = malloc(sizeof(t_token));
	if (!token)
		return (-1);
	if (!value[i])
	{
		free(token);
		return (-1);
	}
	token->type = check_token(value[i]);
	if (token->type >= 1 && token->type <= 4)
	{
		if (value[i + 1])
		{
			token->data = ft_strdup(value[i + 1]);
			if (!token->data)
				ft_putstr_fd("malloc token->data error\n", 1);
		}
		else
		{
			printf("minishell❌: syntax error\n");
			free(token);
			return (-1);
		}
		ret = 1;
	}
	else
		token->data = ft_strdup(value[i]);
	*tmp = token;
	return (ret);
}

void	add_to_list(char **token, t_list *lst)
{
	t_token	*tmp;
	t_token	*last;
	int		index;
	int		status;

	index = 0;
	last = NULL;
	tmp = NULL;
	while (token[index])
	{
		status = add_token(lst->content, token, index, &tmp);
		if (status == 1)
			index += 2;
		else if (status <= 0)
			index++;
		if (last == NULL)
			lst->content = tmp;
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
			while (input[i] != '\'' && input[i] != '\"')
				i++;
		}
		if (input[i] && strchr(WHITESPACE, input[i]))
			space++;
		i++;
	}
	return (space);
}

/*tokenize the input the first time using the "|" as an delimiter*/
char	**tokenize_one(char *input, int pipe)
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
		result[index++] = ft_strdup(token);
		token = ft_strtok(NULL, "|");
	}
	result[index] = NULL;
	return (result);
}

/*This is an extension for the count_pipes function. It handles the pipes 
in case of appearance.*/
void	error_check_pipes(int *i, int *pipe, char *input)
{
	(*i)++;
	if (input[*i] == '\0')
		error_lexer("|", 2);
	while (input[*i] && strchr(WHITESPACE, input[*i]))
	{
		if (input[*i] && strchr(WHITESPACE, input[*i]) && input[*(i)] == '\0')
			error_lexer("|", 2);
		(*i)++;
	}
	if (input[*i] == '|')
		error_lexer("|", 2);
	else
		*pipe += 1;
}

/*count "|"-sections within the input. used for allocating memory*/
int	count_pipes(char *input)
{
	int	i;
	int	pipe;

	i = 0;
	pipe = 0;
	while (input[i] && strchr(WHITESPACE, input[i]))
		i++;
	if (input[i] == '|')
		error_lexer("|", 3);
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
		{
			i++;
			while (input[i] != '\"' && input[i] == '\'')
				i++;
		}
		if (input[i] == '|')
			error_check_pipes(&i, &pipe, input);
		if (input[i])
			i++;
	}
	return (pipe);
}
