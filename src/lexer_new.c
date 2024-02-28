
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@42>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:16:46 by miheider          #+#    #+#             */
/*   Updated: 2024/02/06 21:14:34 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*checks token for <, >, >>, <<!*/
int check_token(char *token)
{
	int len;
	int i;

	if (!token)
		return (-1);
	len = ft_strlen(token);
	i = 0;
	if (token[0] == '<' && len == 1)
		return (1);
	if (token[0] == '>' && token[1] == '<' && len == 2)
		return (2);
	if (token[0] == '>' && len == 1)
		return (3);
	if (token[0] == '>' && token[1] == '>' && len == 2)
		return (4);
	return (0);
}

/*this function checks for special characters, except for pipes*/
int check_char(char *character)
{
	char    *special;
	int     i;

	i = 0;
	special = "*?[]()<>#\"";
	if (*character == '.' || *character == '\'' || *character == '\"')
		return (2);
	while (i < 10)
	{
		if (special[i] == *character)
			return (1);
		i++;
	}
	return (0);
}

/*tokenizing the input; rebuilt the original function strtok and addes a feature: it is ignoring the delimiter in quotes.*/
char    *ft_strtok(char *s1, const char *delim)
{
	static char    *str;
	char           *start;
	int            in_quotes;

	if (s1)
		str = s1;
	if (!str)
		return (0);
	while (*str && ft_strchr(delim, *str))
		str++;
	if (*str == '\0')
		return (0);
	start = str;
	in_quotes = 0;
	while (*str)
	{
		if (*str == '.')
			in_quotes = !in_quotes;
		if (!in_quotes && ft_strchr(delim, *str))
			break;
		str++;
	}
	if (*str != '\0')
	{
		*str = '\0';
		str++;
	}
	return (start);
}

void	add_token(t_token **lst, char **value, int *i)
{
	t_token	*token;
	t_token	*tmp;

	token = malloc(sizeof(t_token));
	if (!token)
		return ;
	token->type = check_token(value[*i]);
	if (token->type >= 1 && token->type <= 4)
	{
		if (value[*i + 1])
			token->data = value[++(*i)];
		else
			printf("minishell: syntax error");
	}
	else
		token->data = value[*i];
//	printf("%s ", token->data);
	token->next = NULL;
	if (!*lst)
		*lst = token;
	else
	{
		tmp = *lst;
		while (tmp->next)
		{
			tmp = tmp->next;
		}
		tmp->next = token;
	}
}

void add_to_list(char **token, t_list *lst)
{
	t_token	*tmp;
	int 	index;

	tmp = lst->content;
	index = 0;
	while (token[index])
	{
		add_token(&tmp, token, &index);
		index++;
	}
}

char    **tokenize_input(char *input)
{
	char **result = NULL;
	char *token;
	int index = 0;

	token = ft_strtok(input, " ");
	while (token != NULL) {
		result = realloc(result, (index + 1) * sizeof(char *));
		if (result == NULL) {
			fprintf(stderr, "Memory allocation failed\n");
			return (NULL);
		}
		result[index++] = ft_strdup(token);
		token = ft_strtok(NULL, " ");
	}
	return (result);
//caigner
//	free(result); // Free array of tokens
}

char **set_up_array(int wc, int cc, char *input)
{
	char *new_string;
	int i = 0;
	int j = 0;

	new_string = (char *)malloc(sizeof(char) * (wc + cc));
	if (!new_string)
	{
		printf("Memory allocation failed!\n");
		exit(1);
	}
	while (input[i] == ' ')
		i++;
	while (input[i])
	{
		while (input[i] && input[i] != ' ')
		{
			if (check_char(&input[i]) == 2)
			{
				if (input[i - 1] != ' ' && i > 0)
				{
					new_string[j] = ' ';
					j++;
				}
				new_string[j] = input[i];
				i++;
				j++;
				while (check_char(&input[i]) != 2)
				{
					new_string[j] = input[i];
					i++;
					j++;
				}
				if (check_char(&input[i]) == 2)
				{
					new_string[j] = input[i];
					j++;
					i++;
					if (input[i] == ' ')
						break;
					if (input[i] != ' ' && input[i] != '\0')
					{
						new_string[j] = ' ';
						j++;
					}
				}
			}
			if (check_char(&input[i]) == 0 && check_char(&input[i - 1]) == 1)
			{
				new_string[j] = ' ';
				j++;
			}
			if (check_char(&input[i]) == 1 && check_char(&input[i - 1]) == 0 && input[i - 1] != ' ' && i > 0)
			{
				new_string[j] = ' ';
				j++;
			}
			if (check_char(&input[i]) == 1 && check_char(&input[i - 1]) == 1 && input[i] != input[i - 1])
			{
				new_string[j] = ' ';
				j++;
			}
			new_string[j] = input[i];
			i++;
			j++;
		}
		while (input[i] == ' ')
			i++;
		if (input[i - 1] == ' ' && input[i] != '\0')
		{
			new_string[j] = ' ';
			j++;
		}
	}
	new_string[j] = '\0';
	return (tokenize_input(&new_string[0]));
//	free(new_string);
}

char    **prep_input(char *input)
{
	int wc = 0;
	int cc = 0;
	char *string;

	string = input;
	if (check_char(input) == 1)
		wc--;
	while (*input)
	{
		while (*input == ' ')
			input++;
		if (*input != ' ')
		{
			if (*input == '\0')
				break;
			wc++;
		}
		while (*input && *input != ' ')
		{
			if (check_char(input) == 1 && ((*(input - 1) == ' ') || (*(input - 1) == *input)))
				cc++;
			else if (check_char(input) == 1 && *(input - 1) != ' ')
			{
				wc++;
				cc++;
			}
			else if (check_char(input) == 0 && (check_char(input - 1) == 1 || check_char(input - 1) == 2))
			{
				wc++;
				cc++;
			}
			else if (check_char(input) == 0 && (check_char(input - 1) != 1))
				cc++;
			else
			{
				if (*(input - 1) != ' ')
					wc++;
				cc++;
				input++;
				while (check_char(input) != 2 && *input)
				{
					cc++;
					input++;
				}
				cc++;
			}
			input++;
		}
	}
	return (set_up_array(wc, cc, string));
}

/*tokenize the input the first time using the "|" as an delimiter*/
char    **tokenize_one(char *input, int pipe)
{
	char    **result;
	char    *token;
	int     index;

	result = malloc((pipe + 1) * sizeof(char *));
	if (result == NULL)
	{
		printf("Error - malloc - tokenize_one\n");
		return (NULL);
	}
	token = ft_strtok(input, "|");
	index = 0;
	while (token != NULL && index <= pipe + 1)
	{
		result[index++] = ft_strdup(token);
		token = ft_strtok(NULL, "|");
	}
	return (result);
//caigner
/* 
	i = 0;
	while (i < pipe + 1)
	{
		prep_input(result[i]);
		i++;
	}                                   
	for (i = 0; i < index; i++)                         //loop for testing only
	{
		printf("result[%d]: ___%s___\n", i, result[i]);
		free(result[i]);
	}
	free(result); */
}

/*count "|"-sections within the input. used for allocating memory*/
int    count_pipes(char *input)
{
	int i = 0;
	int pipe = 0;

	while (input[i])
	{
		if (input[i] == '.')
		{
			i++;
			while (input[i] != '.')
				i++;
		}
		while (input[i] == '|' && input[i + 1] != '|')
		{
			pipe += 1;
			i++;
		}
		i++;
	}
//	printf("pipes: %d\n", pipe);
	return (pipe);
//    tokenize_one(input, pipe); caigner
}
