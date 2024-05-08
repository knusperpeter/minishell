/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:09:24 by miheider          #+#    #+#             */
/*   Updated: 2024/04/28 19:28:00y miheider         ###   ########.fr       */
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
	special = "[]()<>#";
	if (*character == '\'' || *character == '\"')
		return (2);
	while (i < 8)
	{
		if (special[i] == *character)
			return (1);
		i++;
	}
	return (0);
}

int is_delim(char c, const char *delim) 
{
    while (*delim) 
	{
        if (c == *delim)
            return (1);
        delim++;
    }
    return (0);
}

/*This functions handles the quotes section for the ft_strtok function*/
char *quotes_in_strtok(char *str, const char *delim) 
{
    int		in_quotes;
    char	quote_type; 

	in_quotes = 0;
	quote_type = '\0';
    while (*str) 
	{
        if (*str == '\"' && quote_type != '\'') 
		{
            in_quotes = !in_quotes;
            if (!in_quotes)
                quote_type = '\0'; // Reset the quote type
            else
                quote_type = '\"'; // Inside double quotes
        } 
		else if (*str == '\'' && quote_type != '\"') 
		{
            in_quotes = !in_quotes;
            if (!in_quotes)
                quote_type = '\0'; // Reset the quote type
            else
                quote_type = '\''; // Inside single quotes
        }
        if (!in_quotes && is_delim(*str, delim))
            break;
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
//	printf("%s\n", (char *)token);
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
			//error_lexer((char *)token, ft_strlen((char *)token));
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
		if (input[i] && strchr(WHITESPACE, input[i]))
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
int	no_space_array(int *i, int *j, char *input, char *new_string)
{
	if (input[*i] && check_char(&input[*i]) == 2)
	{
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
	if (i > 0 && *cc != 0 && input[i] && !strchr(WHITESPACE, input[i])
		&& strchr(WHITESPACE, input[i - 1]) && (input[i] != '\'' || input[i] != '\"'))
		(*cc)++;
	if (i > 0 && *cc != 0 && input[i] && strchr(WHITESPACE, input[i])
		&& (input[i + 1] == '\'' || input[i + 1] == '\"'))
		(*cc)++;
	if (input[i] && !strchr(WHITESPACE, input[i]))
		(*cc)++;
	if (input[i] == '<' && (input[i + 1] == '\'' || input[i + 1] == '\"'))
		(*cc)--;
}

int	prep_input_two(int *i, int *cc, char *input)
{
	if (*i > 0 && !strchr(WHITESPACE, input[*(i) - 1]))
		(*cc)--;
	count_up(i, cc);
	if (*i > 1 && !strchr(WHITESPACE, input[*(i) - 2]))
		(*cc)++;
	while (input[*i] != '\0' && input[*i] != '\'' && input[*i] != '\"')
	{
		if (input[*(i)] == '\0') // Check for null terminator
			break;
		count_up(i, cc);
	}
	if (input[*i] != '\0')
	{
		if (input[*(i) + 1] != '\0' || !strchr(WHITESPACE, input[*(i) + 1]))
			count_up(i, cc);
		else if (input[*(i) + 1] != '\0')
			return (-9);
	}	
	return (9);
}

void	prep_input_one(int i, int *cc, char *input)
{
	if (i > 0 && *cc != 0 && check_char(&input[i]) == 1
			&& !strchr(WHITESPACE, input[i - 1]) && input[i] != input[i - 1]
			&& (input[i - 1] != '\'' || input[i - 1] != '\"'))
		(*cc)++;
	if (input[i] && check_char(&input[i]) == 1 && !strchr(WHITESPACE, input[i + 1])
		&& check_char(&input[i + 1]) != 1 && input[i + 1] != '\0'
		&& (input[i] != '\'' || input[i] != '\"'))
		(*cc)++;
}

char	**prep_input(char *input)
{
	int	i;
	int	cc;

	i = 0;
	cc = 0;
	while (input[i] && strchr(WHITESPACE, input[i]))
		i++;
	while (input[i])
	{
		prep_input_one(i, &cc, input);
		if (input[i] && (input[i] == '\'' || input[i] == '\"'))
			if (prep_input_two(&i, &cc, input) < 0)
				break ;
		prep_input_three(i, &cc, input);
		i++;
	}
	return (set_up_array(cc, input));
}

/*prints out the correct error message when only > or < as input*/
int check_this(t_common *c, char *result, int j)
{
	int	status;

	status = 0;
	if (*result == '<' || *result == '>')
	{
		if (j == 6 && *result == '<')
			status = ft_putstr_fd("❌ minishell: syntax error near unexpected token `<<<'\n", 2);
		else if (j == 6 && *result == '>')
			status = ft_putstr_fd("❌ minishell: syntax error near unexpected token `>>>'\n", 2);
		else if (j == 5 && *result == '<')
			status = ft_putstr_fd("❌ minishell: syntax error near unexpected token `<<'\n", 2);
		else if (j == 5 && *result == '>')
			status = ft_putstr_fd("❌ minishell: syntax error near unexpected token `>>'\n", 2);
		else if (j == 4 && *result == '<')
			status = ft_putstr_fd("❌ minishell: syntax error near unexpected token `<'\n", 2);
		else if (j == 4 && *result == '>')
			status = ft_putstr_fd("❌ minishell: syntax error near unexpected token `>'\n", 2);
		else if (j == 3 || (j == 2 && result [1] == '<' && result[2] == '\0') || result[1] == '\0')
			status = ft_putstr_fd("❌ minishell: syntax error near unexpected token `newline'\n", 2);
		if (status)
		{
			c->exitstatus = 2;
			ft_clean_exit(c, NULL, 0);
			exit (2);
		}
	}						// free stuff
	return (0);
}

/*this function checks for dots as only input*/
int	check_dot(char *result, int k, int j)
{
	int i;

	if (result[k] == '.')
	{
		if(result[k] && result[k] == '.' && result [k + 1] == '\0')
		{
			ft_putstr_fd("minishell: .: filename argument required\n.: usage: . filename [arguments]", 2);
			exit (2);
		}
		if (j > 1)
		{
			i = 0;
			while (result[i])
			{
				write (2, &result[i], 1);
				i++;
			}
			ft_putstr_fd(": command not found\n", 2);
			exit (127);
		}
	}
	return (0);	
}

/*this function checks for empty quote input*/
int	check_quotes(char *result, int k)
{
	int i;
	
	if (result[k] == '"')
		k++;
	while (result[k] != '"')
	{
		if (result[k] == ' ' || (result[k] >= 9 && result[k] <= 13))
			k++;
		else
			return (0);
		k++;
	}
	if (result[k + 1] == '"')
		k++;
	i = 0;
	printf("%d\n", k);
	while (i < k + 1 /*&& (result[i] != ' ' || (result[k] >= 9 && result[k] <= 13))*/)
	{
		write (2, &result[i], 1);
		printf("result[%d]: %c\n", i, result[i]);
		i++;
	}
	ft_putstr_fd (" :command not found\n", 2);
	return (0);	
}

int	check_again(t_common *c, char *result, int k, char fir)
{
	int status;
	int len;

	status = 0;
	len = ft_strlen(result);
	if ((len >= 2 && fir != '<' && fir != '>') || (len >= 2 && result[1] != ' '))
		return (0);
	if (len >= 2 && (result[k + 1] == ' ' || (result[k + 1] >= 9 || result[k + 1] <= 13)))
	{
		k++;
		while(result[k] == ' ' || (result[k] >= 9 && result[k] <= 13))
			k++;
	}
	else
		return (0);
	if (result[k] == '<' || result[k] == '>')
	{
		status = ft_putstr_fd("❌ minishell: syntax error near unexpected token `", 2);
		if (result[k] == '>')
			write(2, &result[k], 1);
		else
			write(2, "<", 1);
		if (result [k + 1] == '<' || result[k + 1] == '>')
		{
			if (result[k + 1] == '>')
				write(2, &result[k], 1);
			else
				write(2, "<", 1);
		}
		write(2, "'", 1);
	}
	else
		return (0);	
	if (status)
	{
	    c->exitstatus = 2;
	    ft_clean_exit(c, NULL, 0);
	    exit (2);
	}
							// free stuff*/
	return (0);
}

int	check_more(t_common *c, char *result, int k, char fir)
{
	int len;
	int	status;

	len = ft_strlen(result);
	if (!(len >= 4 && result[k] == result[k + 1] && result[k] == fir && result[k + 2] == ' ' && (result[k + 3] == '<' ||  result[k + 3] == '>')))
		return (0);
	if (result[k + 3] == '<' || result[k + 3] == '>')
	{
		status = ft_putstr_fd("❌ minishell: syntax error near unexpected token `", 2);
		if (result[k] == '>')
			write(2, &result[k + 3], 1);
		else
			write(2, "<", 1);
		if (len >= 5 && (result [k + 4] == '<' || result[k + 4] == '>'))
		{
			if (result[k + 4] == '>')
				write(2, &result[k], 1);
			else
				write(2, "<", 1);
		}
		write(2, "'", 1);
	}
	else
		return (0);	
	if (status)
	{
	    c->exitstatus = 2;
	    ft_clean_exit(c, NULL, 0);
	    exit (2);
	}							// free stuff
	return (0);
}

int	check_that(t_common *c, char *result, int k)
{
	int	status;

	status = 0;
	if (result[k] == '<' && result[k + 1] == '>')
	{
		k += 2;
		while (result[k] == ' ' || (result[k] >= 9 && result[k] <= 13) || result[k] == '\0')
		{
			if (result[k] == '\0')
			{
				status = ft_putstr_fd("❌ minishell: syntax error near unexpected token `newline'\n", 2);
				break;
			}
			else
				k++;
		}
	}
	if (status)
	{
	    c->exitstatus = 2;
	    ft_clean_exit(c, NULL, 0);
	    exit (2);
	}							// free stuff
	return (0);
}

int check_result(t_common *c, char *result)
{
	int		i;
	int 	j;
	int 	k;
	char	fir;
	int 	len;

	k = 0;
	i = 0;
	len = ft_strlen(result);
	while (len >= 1 && (result[i] == ' ' || (result[i] >= 9 && result[i] <= 13)))
		i++;
	if (len >= 1 && (result[i] == '<' || result[i] == '>'))
		fir = result[i];
	else if (result[i] == '.' && (result[i + 1] == '\0' || (len >= 2 && result[i + 1] == '.')))
		fir = result[i];
	else if (result[i] == '\"')
		fir = result[i];
	else
		return (0);
	k = i;
	j = 0;
	while (result[i] && i < k + 6)
	{
		if (result[i] == fir)
			j++;
		else
			break;
		i++;	
	}
	check_that(c, &result[k], k);
	if (j > 0)
	{	
		check_this(c, &result[k], j);
		check_dot(&result[0], k, j);
		check_quotes(&result[k], k);
		check_again(c, &result[k], k, fir);
		check_more(c, &result[0], k, fir);
	}
	return (0);
}

int	check_empty_line (t_common *c, char *input, int pipe)
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
		result[index++] = ft_strdup(token);
		token = ft_strtok(NULL, "|");
	}
	result[index] = NULL;
	if (check_empty_line(c, input, pipe))
		exit (1);								// free und give promt back
	check_result(c, result[0]);
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

int	is_in_quotes(char *str, int i)
{
	int	inside_quotes = 0;
	char	current_quote = '\0';

	while (i >= 0)
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (current_quote == str[i])
			{
				inside_quotes = !inside_quotes;
				current_quote = '\0';
			}
			else if (current_quote == '\0')
			{
				inside_quotes = !inside_quotes;
				current_quote = str[i];
			}
		}
		i--;
	}
	return inside_quotes;
}

int	ignore_pipe(char *str, int i)
{
	if (i == 0)
		return (0);
	if (is_in_quotes(str, i))
		return (1);
	return (0);
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
	if (input[i] == '|' && !ignore_pipe(input, i))
		error_lexer("|", 3);
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
		{
			i++;
			while (input[i] != '\"' && input[i] == '\'')
				i++;
		}
		if (input[i] == '|' && !ignore_pipe(input, i))
			error_check_pipes(&i, &pipe, input);
		if (input[i])
			i++;
	}
	return (pipe);
}
