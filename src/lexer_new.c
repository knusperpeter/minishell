/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/03/19 15:53:31 by caigner          ###   ########.fr       */
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
	special = "*?[]()<>#";
	if (*character == '\'' || *character == '\"')
		return (2);
	while (i < 10)
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
			while (input[i] != '\'' && input[i] != '\"') //input[i] sonst segfault wenn unclosed quotes
				i++;
		}
		if (input[i] == ' ')
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
	return (result);
}

/*this is the inner while loop of the set_up_array function. The first 
if statement is handling quotes, the second one is handling different 
situations to inpute a space to new_string*/
int	no_space_array(int *i, int *j, char *input, char *new_string)
{
	if (check_char(&input[*i]) == 2)
	{
		new_string[(*j)++] = input[(*i)++];
		while (input[*i] && check_char(&input[*i]) != 2)
			new_string[(*j)++] = input[(*i)++];
		if (input[*i] && check_char(&input[*i]) == 2)
		{
			new_string[(*j)++] = input[(*i)++];
			if (input[*i] == ' ')
				return (-10);
		}
	}
	if ((i > 0 && check_char(&input[*i]) == 0
			&& check_char(&input[*(i) - 1]) == 1)
		|| (check_char(&input[*i]) == 1
			&& check_char(&input[*(i) - 1]) == 0
			&& input[(*i) - 1] != ' ') || (check_char(&input[*i]) == 1
			&& check_char(&input[*(i) - 1]) == 1
			&& input[*i] != input[*(i) - 1]))
		new_string[(*j)++] = ' ';
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
	while (input[i] == ' ')
		i++;
	while (input[i])
	{
		while (input[i] && input[i] != ' ')
		{
			if (no_space_array(&i, &j, input, new_string) < 0)
				break ;
		}
		while (input[i] && input[i] == ' ')
			i++;
		if (input[i - 1] == ' ' && input[i] != '\0')
			new_string[j++] = ' ';
	}
	new_string[j] = '\0';
	return (tokenize_input(new_string));
}

/*counting characters (cc) from input and taking care of quotes. This is
used for mallocing memory in the 'set_up_array' function.*/
void	count_up(int *i, int *cc)
{
	(*i)++;
	(*cc)++;
}

/*the prep_input functions 0-4 are are counting the characters and spaces
beeded to allocate memory for the norminized string.*/
void	prep_input_three(int i, int *cc, char *input)
{
	if (i > 0 && *cc != 0 && input[i] && input[i] != ' '
		&& input[i - 1] == ' ' && (input[i] != '\'' || input[i] != '\"'))
		(*cc)++;
	if (i > 0 && *cc != 0 && input[i] && input[i] == ' '
		&& (input[i + 1] == '\'' || input[i + 1] == '\"'))
		(*cc)++;
	if (input[i] && input[i] != ' ')
		(*cc)++;
	if (input[i] == '<' && (input[i + 1] == '\'' || input[i + 1] == '\"'))
		(*cc)--;
}

int	prep_input_two(int *i, int *cc, char *input)
{
	if (input[*(i) - 1] != ' ')
		(*cc)--;
	count_up(i, cc);
	if (*i > 1 && input[*(i) - 2] != ' ')
		(*cc)++;
	while (input[*i] != '\'' && input[*i] != '\"')
		count_up(i, cc);
	if (input[*(i) + 1] != '\0' || input[*(i) + 1] == ' ')
		count_up(i, cc);
	else if (input[*(i) + 1] != '\0')
		return (-9);
	return (9);
}

void	prep_input_one(int i, int *cc, char *input)
{
	if (i > 0 && *cc != 0 && input[i] == '<' && input[i - 1] != ' '
		&& input[i - 1] != '<' && (input[i - 1] != '\''
			|| input[i - 1] != '\"'))
		(*cc)++;
	if (input[i] == '<' && input[i + 1] != ' ' && input[i + 1] != '<'
		&& input[i + 1] != '\0' && (input[i] != '\'' || input[i] != '\"'))
		(*cc)++;
}

char	**prep_input(char *input)
{
	int	i;
	int	cc;

	i = 0;
	cc = 0;
	while (input[i] == ' ')
		i++;
	while (input[i])
	{
		prep_input_one(i, &cc, input);
		if (i > 0 && input[i] && (input[i] == '\'' || input[i] == '\"'))
			if (prep_input_two(&i, &cc, input) < 0)
				break ;
		prep_input_three(i, &cc, input);
		i++;
	}
	return (set_up_array(cc, input));
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
	while (input[*i] && input[*i] == ' ')
	{
		if (input[*i] == ' ' && input[*(i + 1)] == '\0')
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
	while (input[i] && input[i] == ' ')
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
