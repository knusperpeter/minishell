/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 15:59:40 by miheider          #+#    #+#             */
/*   Updated: 2024/05/15 19:47:37y miheider         ###   ########.fr       */
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
	special = "()<>";
	if (*character == '\'' || *character == '\"')
		return (2);
	while (i < 5)
	{
		if (special[i] == *character)
			return (1);
		i++;
	}
	return (0);
}

int	is_delim(char c, const char *delim)
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
char	*quotes_in_strtok(char *str, const char *delim)
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
				quote_type = '\0';
			else
				quote_type = '\"';
		}
		else if (*str == '\'' && quote_type != '\"')
		{
			in_quotes = !in_quotes;
			if (!in_quotes)
				quote_type = '\0';
			else
				quote_type = '\'';
		}
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

void	init_add_to_list(t_token **last, t_token **tmp, int *index)
{
	*index = 0;
	*last = NULL;
	*tmp = NULL;
}

void	add_to_list(char **token, t_list *lst)
{
	t_token	*tmp;
	t_token	*last;
	int		index;
	int		status;

	init_add_to_list(&last, &tmp, &index);
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
int	no_space_array(int *i, int *j, char *input, char *new_string)
{
	static int	quote;

	if (input[*i] && check_char(&input[*i]) == 2)
	{
		quote++;
		if (quote % 2 == 1 && (input[(*i) - 1] == '<'
				|| input[(*i) - 1] == '>'))
		{
			new_string[(*j)++] = ' ';
			new_string[(*j)++] = input[(*i)++];
		}
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
	}
	if (*i > 0)
	{
		if ((input[*i] && check_char(&input[*i]) == 0
				&& check_char(&input[*(i) - 1]) == 1)
			|| (check_char(&input[*i]) == 1
				&& check_char(&input[*(i) - 1]) == 0
				&& !ft_strchr(WHITESPACE, input[(*i) - 1]))
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
	while (input[i] && ft_strchr(WHITESPACE, input[i]))
		i++;
	while (input[i])
	{
		while (input[i] && !ft_strchr(WHITESPACE, input[i]))
		{
			if (no_space_array(&i, &j, input, new_string) < 0)
				break ;
		}
		while (input[i] && ft_strchr(WHITESPACE, input[i]))
			i++;
		if (i > 0)
			if (input[i] != '\0' && ft_strchr(WHITESPACE, input[i - 1]))
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
	if (i > 0 && *cc != 0 && input[i] && !ft_strchr(WHITESPACE, input[i])
		&& ft_strchr(WHITESPACE, input[i - 1])
		&& (input[i] != '\'' || input[i] != '\"'))
		(*cc)++;
	if (i > 0 && *cc != 0 && input[i] && ft_strchr(WHITESPACE, input[i])
		&& (input[i + 1] == '\'' || input[i + 1] == '\"'))
		(*cc)++;
	if (input[i] && !ft_strchr(WHITESPACE, input[i]))
		(*cc)++;
	if (input[i] == '<' && (input[i + 1] == '\'' || input[i + 1] == '\"'))
		(*cc)--;
}

int	prep_input_two(int *i, int *cc, char *input)
{
	if (*i > 0 && !ft_strchr(WHITESPACE, input[*(i) - 1]))
		(*cc)--;
	count_up(i, cc);
	if (*i > 1 && !ft_strchr(WHITESPACE, input[*(i) - 2]))
		(*cc)++;
	while (input[*i] != '\0' && input[*i] != '\'' && input[*i] != '\"')
	{
		if (input[*(i)] == '\0')
			break ;
		count_up(i, cc);
	}
	if (input[*i] != '\0')
	{
		if (input[*(i) + 1] != '\0' || !ft_strchr(WHITESPACE, input[*(i) + 1]))
			count_up(i, cc);
		else if (input[*(i) + 1] != '\0')
			return (-9);
	}
	return (9);
}

void	prep_input_one(int i, int *cc, char *input)
{
	if (i > 0 && *cc != 0 && check_char(&input[i]) == 1
		&& !ft_strchr(WHITESPACE, input[i - 1]) && input[i] != input[i - 1]
		&& (input[i - 1] != '\'' || input[i - 1] != '\"'))
		(*cc)++;
	if (input[i] && check_char(&input[i]) == 1
		&& !ft_strchr(WHITESPACE, input[i + 1])
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
	while (input[i] && ft_strchr(WHITESPACE, input[i]))
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

void	es_cul(t_common *c, int es)
{
	c->exitstatus = es;
	ft_cleanup_loop(c);
}

/*prints out the correct error message when only > or < as input*/
int	check_this(t_common *c, char *result, int j)
{
	int	status;

	status = 0;
	if (*result == '<' || *result == '>')
	{
		if (j == 6 && *result == '<')
			status = ft_putstr_fd(MESSAGE1, 2);
		else if (j == 6 && *result == '>')
			status = ft_putstr_fd(MESSAGE2, 2);
		else if (j == 5 && *result == '<')
			status = ft_putstr_fd(MESSAGE3, 2);
		else if (j == 5 && *result == '>')
			status = ft_putstr_fd(MESSAGE4, 2);
		else if (j == 4 && *result == '<')
			status = ft_putstr_fd(MESSAGE5, 2);
		else if (j == 4 && *result == '>')
			status = ft_putstr_fd(MESSAGE6, 2);
		else if (j == 3 || (j == 2 && result [1] == '<' && result[2] == '\0')
			|| (j == 2 && result [1] == '>' && result[2] == '\0')
			|| result[1] == '\0')
			status = ft_putstr_fd(MESSAGE7, 2);
		if (status)
			return (es_cul(c, 2), 1);
	}
	return (0);
}

/*this function checks for dots as only input*/
int	check_dot(t_common *c, char *result, int k, int j)
{
	int	i;

	if (result[k] == '.')
	{
		if (result[k] && result[k] == '.' && result [k + 1] == '\0')
		{
			ft_putstr_fd(MESSAGE8, 2);
			return (es_cul(c, 2), 1);
		}
		else if (j > 1)
		{
			i = 0;
			while (result[i])
			{
				write (2, &result[i], 1);
				i++;
			}
			ft_putstr_fd(": command not found\n", 2);
			return (es_cul(c, 127), 1);
		}
	}
	return (0);
}

/*this function checks for empty quote input*/
int	check_quotes(t_common *c, char *result, int k)
{
	int	i;

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
	while (i < k + 1)
	{
		write (2, &result[i], 1);
		i++;
	}
	ft_putstr_fd (" :command not found\n", 2);
	return (es_cul(c, 127), 1);
}

int	check_again(t_common *c, char *result, int k, char fir)
{
	int	status;
	int	len;

	status = 0;
	len = ft_strlen(result);
	if ((len >= 2 && fir != '<' && fir != '>')
		|| (len >= 2 && result[1] != ' '))
		return (0);
	if (len >= 2 && (result[k + 1] == ' '
			|| (result[k + 1] >= 9 || result[k + 1] <= 13)))
	{
		k++;
		while (result[k] == ' ' || (result[k] >= 9 && result[k] <= 13))
			k++;
	}
	else
		return (0);
	if (result[k] == '<' || result[k] == '>')
	{
		status = ft_putstr_fd(MESSAGE9, 2);
		if (result[k] == '>')
			write(2, &result[k], 1);
		else
			write(2, "<", 1);
		if (len >= 2 && (result[k + 1] == '<' || result[k + 1] == '>'))
		{
			if (result[k + 1] == '>')
				write(2, &result[k], 1);
			else
				write(2, "<", 1);
		}
		write(2, "'\n", 2);
	}
	else
		return (0);
	if (status)
		return (es_cul(c, 2), 1);
	return (0);
}

int	check_more(t_common *c, char *result, int k, char fir)
{
	int	len;
	int	status;

	len = ft_strlen(result);
	if (!(len >= 4 && result[k] == result[k + 1] && result[k] == fir
			&& result[k + 2] == ' ' && (result[k + 3] == '<'
				|| result[k + 3] == '>')))
		return (0);
	if (result[k + 3] == '<' || result[k + 3] == '>')
	{
		status = ft_putstr_fd(MESSAGE9, 2);
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
		write(2, "'\n", 2);
	}
	else
		return (0);
	if (status)
		return (es_cul(c, 2), 1);
	return (0);
}

int	check_that(t_common *c, char *result, int k)
{
	int	status;

	status = 0;
	if (result[k] == '<' && result[k + 1] == '>')
	{
		k += 2;
		while (result[k] == ' ' || (result[k] >= 9
				&& result[k] <= 13) || result[k] == '\0')
		{
			if (result[k] == '\0')
			{
				status = ft_putstr_fd(MESSAGE7, 2);
				break ;
			}
			else
				k++;
		}
	}
	if (status)
		return (es_cul(c, 2), 1);
	return (0);
}

int	check_one_more(t_common *c, char *result)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (result[i] != '\0')
	{
		if (result[i] == '>')
		{
			i++;
			while (result[i] == ' ' || (result[i] >= 9 && result[i] <= 13))
				i++;
			/*if (result[i] == '\0' || (result[i] == ' '
					|| (result[i] >= 9 && result[i] <= 13)))
			{
				status = ft_putstr_fd(MESSAGE7, 2);
				break ;
			}*/
			if (result[i] == '>')
			{
				i++;
				while (result[i] == ' ' || (result[i] >= 9 && result[i] <= 13))
					i++;
				if (result[i] == '\0' || (result[i] == ' '
						|| (result[i] >= 9 && result[i] <= 13)))
				{
					status = ft_putstr_fd(MESSAGE7, 2);
					break ;
				}
			}
		}
		i++;
	}
	if (status)
		return (es_cul(c, 2), 1);
	return (0);
}

void	init_for_open_quotes(int *s_quote, int *d_quote, int *i)
{
	*s_quote = 0;
	*i = 0;
	*d_quote = 0;
}

int	open_quotes(t_common *c, char *result)
{
	int	i;
	int	s_quote;
	int	d_quote;

	init_for_open_quotes(&s_quote, &d_quote, &i);
	c->open_single_quotes = 0;
	c->open_double_quotes = 0;
	while (result[i])
	{
		handle_quote_state(c, result[i]);
		if (result[i] == '\"' && c->open_single_quotes == 0)
			d_quote++;
		else if (result[i] == '\'' && c->open_double_quotes == 0)
			s_quote++;
		i++;
	}
	if (c->open_single_quotes == 1 || c->open_double_quotes == 1)
	{
		ft_putstr_fd("❌ minishell: syntax error dquote\n", 2);
		return (es_cul(c, 2), 1);
	}
	return (0);
}

int	check_sq(t_common *c, char *result, int k)
{
	int	sq;

	sq = 0;
	while (result[k])
	{
		if (result[k] == '\'')
		{
			while (result[k] == '\'')
			{
				sq++;
				k++;
			}
			if (result[k] == ' ' && sq % 2 == 0)
			{
				ft_putstr_fd("❌ minishell: Command '' not found.\n", 2);
				return (es_cul(c, 127), 1);
			}
		}
		else
			return (0);
		k++;
	}
	return (0);
}

int	check_inout(t_common *c, char *result)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (result[i])
	{
		if (result[i] == '>')
		{
			i++;
			while (result[i] == ' ')
				i++;
			if (result[i] == '<')
				status = ft_putstr_fd(MESSAGE5, 2);
		}
		i++;
	}
	if (status)
		return (es_cul(c, 2), 1);
	return (0);
}

int	check_result(t_common *c, char *result)
{
	int		i;
	int		j;
	int		k;
	char	fir;
	int		len;

	k = 0;
	i = 0;
	len = ft_strlen(result);
	if (check_sq(c, &result[k], k))
		return (1);
	if (open_quotes(c, &result[0]))
		return (1);
	if (check_inout(c, &result[0]))
		return (1);
	if (check_one_more(c, &result[0]))
		return (1);
	while (len >= 1 && (result[i] == ' ' || (result[i] >= 9
				&& result[i] <= 13)))
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
	j = 0;
	while (result[i] && i < k + 6)
	{
		if (result[i] == fir)
			j++;
		else
			break ;
		i++;
	}
	if (check_that(c, &result[k], k))
		return (1);
	if (j > 0)
	{
		if (check_this(c, &result[k], j))
			return (1);
		if (check_dot(c, &result[0], k, j))
			return (1);
		if (check_quotes(c, &result[k], k))
			return (1);
		if (check_again(c, &result[k], k, fir))
			return (1);
		if (check_more(c, &result[0], k, fir))
			return (1);
	}
	return (0);
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


		// if (check_empty_line(c, input, pipe))
		// 	return (c->exitstatus = 0, free(result[index]), free(result), NULL);
		// if (check_result(c, result[index]))
		// 	return (free(result[index]), free(result), NULL);




char	**tokenize_one(t_common *c, char *input, int pipe)
{
	char	**result;
	char	*token;
	int		index;
	(void) c;
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
		token = ft_strtok(NULL, "|");
		index++;
	}
	result[index] = NULL;
	return (result);
}







		// token = ft_protect(c, malloc(sizeof(t_token)), 0, 0);







/*tokenize the input the first time using the "|" as an delimiter
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
}*/





/*this function checks the quote ststus of an input string. it starts 
from the beginning of a string untill the actual position. if a single or a double 
quote is open it returns 1, if not it returns 0*/
int	q_status(char *input, int i)
{
	int	k;
	int	dq;
	int	sq;

	k = 0;
	dq = 0;
	sq = 0;
	while (input[k] && k <= i)
	{
		if (input[k] == '\'')
			sq++;
		if (input[k] == '\"')
			dq++;
		k++;
	}
	if (sq % 2 == 0 && dq % 2 == 0)
		return (0);
	else
		return (1);
}


int	check_pipe_redir(t_common *c, char *input)
{
	int	i;
	int	dir;
	
	i = 0;
	dir = 0;
	while (input[i])
	{
		while (input[i] && (input[i] == ' ' || (input[i] >= 9 && input[i] <= 13)))
			i++;
		if ((input[i] == '<' || input[i] == '>') && !q_status(input, i))
		{
			dir = 1;
			i++;
		}
		while (input[i] && (input[i] == ' ' || (input[i] >= 9 && input[i] <= 13)))
			i++;
		if (input[i] == '|' && !q_status(input, i) && dir == 1)
			return (error_lexer(c, 1), 1);
		else
			dir = 0;
		i++;
	}
	return (0);
}

/*if the last printable character of the input is a pipe ('|') it enters the 
error_lexer function and displays an error message (incl freeing)*/
int	check_pipe_error_last(t_common *c, char *input, int len)
{
	while (len > 0 && (input[len - 1] == ' '
			|| (input[len - 1] >= 9 && input[len - 1] <= 13)))
		len--;
	if (input[len - 1] == '|')
		return (error_lexer(c, 1), 1);
	return (0);
}

/*this function checks if an pipe ('|') related syntax error occurs and enters the 
error_lexer function and displays an error message (incl freeing) if this is the case*/
int	check_pipe_error(t_common *c, char *input, int len)
{
	int	i;
	int	pipe;

	i = 0;
	pipe = 0;
	while (input[i] && (input[i] == ' ' || (input[i] >= 9 && input[i] <= 13)))
		i++;
	if (i + 1 < len && input[i] == '|'
		&& input[i + 1] == '|' && !q_status(input, i))
		return (error_lexer(c, 2), -1);
	if (i < len && input[i] == '|' && !q_status(input, i))
		return (error_lexer(c, 1), -1);
	while (input[i])
	{
		if (input[i++] == '|' && !q_status(input, i))
		{
			pipe = 1;
			while (input[i] == ' ' || (input[i] >= 9 && input[i] <= 13))
				i++;
		}
		if (input[i] == '|' && input[i - 1] == '|' && !q_status(input, i + 1))
			return (error_lexer(c, 2));
		if (input[i] == '|' && !q_status(input, i) && pipe == 1)
			return (error_lexer(c, 1));
		pipe = 0;
		if (input[i] != '\0')
			i++;	
	}
	return (0);
}

/*this function counts all the pipes ('|') which are not in quotes. it 
checks for pipe ('|') related errors and returns -1 in case of an error*/
int	count_pipes(t_common *c, char *input)
{
	int	i;
	int	pipe;
	int	len;
	int	in_quote;

	i = 0;
	pipe = 0;
	in_quote = 0;
	len = ft_strlen(input);
	if (check_pipe_error(c, input, len) != 0
		|| check_pipe_error_last(c, input, len) != 0
		|| check_pipe_redir(c, input) != 0)
		return (-1);
	while (input[i])
	{
		in_quote = q_status(&input[0], i);
		if (input[i] == '|' && in_quote == 0)
			pipe++;
		i++;
	}
	return (pipe);
}
























/*
This is an extension for the count_pipes function. It handles the pipes 
in case of appearance.
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

int	is_in_quotes(char *str, int i)
{
	int		inside_quotes;
	char	current_quote;

	inside_quotes = 0;
	current_quote = '\0';
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
	return (inside_quotes);
}

int	ignore_pipe(char *str, int i)
{
	if (i == 0)
		return (0);
	if (is_in_quotes(str, i))
		return (1);
	return (0);
}

count "|"-sections within the input. used for allocating memory
int	count_pipes_cont(t_common *c, char *input, int *i, int *pipe)
{
	while (input[*i])
	{
		if (input[*i] == '\"' || input[*i] == '\'')
		{
			(*i)++;
			while (input[*i] != '\"' && input[*i] == '\'')
				(*i)++;
		}
		if (input[*i] == '|' && !ignore_pipe(input, *i))
		{
			if (error_check_pipes(c, i, pipe, input))
				return (1);
		}
		if (input[*i] != '\0')
			(*i)++;
	}
	return (0);
}

int	count_pipes(t_common *c, char *input)
{
	int	i;
	int	pipe;

	i = 0;
	pipe = 0;
	if (input[ft_strlen(input) - 1] == '|')
	{
		if (error_lexer(c, "|", 3))
			return (-1);
	}
	while (input[i] && ft_strchr(WHITESPACE, input[i]))
		i++;
	if (input[i] == '|' && !ignore_pipe(input, i))
	{
		if (error_lexer(c, "|", 3))
			return (-1);
	}
	if (count_pipes_cont(c, input, &i, &pipe))
		return (-1);
	return (pipe);
}
*/