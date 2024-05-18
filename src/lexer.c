/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:36:38 by miheider          #+#    #+#             */
/*   Updated: 2024/05/18 15:00:57 by miheider         ###   ########.fr       */
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

void	count_up(int *i, int *cc)
{
	(*i)++;
	(*cc)++;
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

/*this function skipps whitespasces. depending on dir it skips whitespace
in forewards direction (dir = 0) or backwards (dir = 1).*/
int	skip_whitespace(char *input, int i, int dir)
{
	if (dir == 0)
	{
		while (input[i] && (input[i] == ' '
				|| (input[i] >= 9 && input[i] <= 13)))
			i++;
	}
	if (dir == 1)
	{
		while (i > 0 && (input[i] == ' '
				|| (input[i] >= 9 && input[i] <= 13)))
			i--;
	}
	return (i);
}

/*this function checks the quote ststus of an input string. it starts 
from the beginning of a string untill the actual position. if a 
single or a double quote is open it returns 1, if not it returns 0*/
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

/*this function checks the situation if a <, <<, > or >> occours. depending if
a space afterwards is present it returns different values.*/
int	check_the_char(char *input, int i, int len)
{
	if (i + 1 <= len && ((input[i] == '>' && input[i + 1] == '>')
			|| (input[i] == '<' && input[i + 1] == '<')))
	{
		if (i + 2 <= len && (input[i + 2] == ' '
				|| (input[i + 2] >= 9 && input[i + 2] <= 13)))
			return (1);
		else
			return (2);
	}
	if (i <= len && (input[i] == '>' || input[i] == '<'))
	{
		if (i + 1 <= len && (input[i + 1] == ' '
				|| (input[i + 1] >= 9 && input[i + 1] <= 13)))
			return (3);
		else
			return (4);
	}
	return (0);
}

/*this function is used by the set_up_array function and checks if a space
before a < or > is needed.*/
int	check_space_before(char *input, int k)
{
	if (input[k] == '>')
	{
		if (input[k - 1] != ' ' && input[k - 1] != '>')
			return (1);
	}
	if (input[k] == '<')
	{
		if (input[k - 1] != ' ' && input[k - 1] != '<')
			return (1);
	}
	return (0);
}

/*this function is used by the set_up_array function and checks if a space
after a < or > is needed.*/
int	check_space_after(char *input, int k)
{
	if (input[k] == '>')
	{
		if (input[k + 1] != ' ' && input[k + 1] != '>')
			return (1);
		else if (input[k + 1] == '>' && (input[k + 2] != ' '
				&& input[k + 2] != '\0'))
			return (1);
	}
	if (input[k] == '<')
	{
		if (input[k + 1] != ' ' && input[k + 1] != '<')
			return (1);
		else if (input[k + 1] == '<' && (input[k + 2] != ' '
				&& input[k + 2] != '\0'))
			return (1);
	}
	return (0);
}

/*this function is writing the characters of the token into the allocated
memmory. it skips multiple spaces and inserts spaces if needed. the new_string
which is passed to the tokenize_input function.*/
char	**set_up_array(t_common *c, int cc, char *input)
{
	char	*new_string;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	new_string = ft_protect(c, (char *)malloc(sizeof(char) * (cc)), 0, 0);
	i = skip_whitespace(input, i, 0);
	k = i;
	while (input[k] && j >= cc - 1)
	{
		if (q_status(input, k) == 0)
		{
			k = skip_whitespace(input, k, 0);
			if (k > 0 && k != i && input[k - 1] == ' ' && input[k] != '\0'
				&& new_string[j - 1] != ' ')
				new_string[j++] = ' ';
			if (input[k] == '<' || input[k] == '>')
			{
				if (k > i && check_space_before(input, k) == 1
					&& new_string[j - 1] != ' ')
					new_string[j++] = ' ';
				new_string[j++] = input[k];
				if (input[k + 1] == input[k])
				{
					new_string[j++] = input[k];
					k++;
				}
				if (check_space_after(input, k) == 1
					&& input[k + 1] != '\0' && input[k + 1] != ' ')
					new_string[j++] = ' ';
			}
			else
				new_string[j++] = input[k];
		}
		else
			new_string[j++] = input[k];
		k++;
	}
	new_string[j] = '\0';
	return (tokenize_input(new_string));
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

/*tis function is used by the prep_input function and manipulates the character
counter according to the situation. it also moves the index representatively.*/
void	update_counts(int *i, int *cc, char *input, int num)
{
	if (*i > 0 && input[*i - 1] != ' ')
		counting_up(i, cc, (num / 1000), (num % 1000) / 100);
	else
		counting_up(i, cc, (num % 100) / 10, num % 10);
}

/*this function is analysing the token and counts the characters needed for
memory allocation in the set_up_array function.*/
char	**prep_input(t_common *c, char *input)
{
	int	i;
	int	cc;
	int	len;

	i = 0;
	cc = 0;
	len = ft_strlen(input);
	i = skip_whitespace(input, i, 0);
	while (input[i])
	{
		if (q_status(input, i) == 0)
		{
			if (i < len && (input[i] == ' '))
			{
				counting_up(&i, &cc, 1, 1);
				i = skip_whitespace(input, i, 0);
				if (input[i] == '\0')
				{
					if (input[i - 1] == ' ')
						cc--;
					break ;
				}
				continue ;
			}
			if (check_double(input, i, len) == 1)
				cc++;
			if (check_double(input, i, len) == 2)
				cc--;
			if (check_the_char(input, i, len) == 1)
				update_counts(&i, &cc, input, 3433);
			if (check_the_char(input, i, len) == 2)
				update_counts(&i, &cc, input, 2434);
			if (check_the_char(input, i, len) == 3)
				update_counts(&i, &cc, input, 2322);
			if (check_the_char(input, i, len) == 4)
				update_counts(&i, &cc, input, 1323);
			i = skip_whitespace(input, i, 0);
			if (input[i] == '\0')
			{
				if (input[i - 1] == ' ')
					cc--;
				break ;
			}
		}
		counting_up(&i, &cc, 1, 1);
	}
	return (set_up_array(c, cc + 1, input));
}
/*

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
			if (result[i] == '\0' || (result[i] == ' '
					|| (result[i] >= 9 && result[i] <= 13)))
			{
				status = ft_putstr_fd(MESSAGE7, 2);
				break ;
			}
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
	// if (check_sq(c, &result[k], k))
	// 	return (1);
	// if (open_quotes(c, &result[0]))
	// 	return (1);
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
		// if (check_dot(c, &result[0], k, j))
		// 	return (1);
		// if (check_quotes(c, &result[k], k))
		// 	return (1);
		if (check_again(c, &result[k], k, fir))
			return (1);
		if (check_more(c, &result[0], k, fir))
			return (1);
	}
	return (0);
}*/

/*this function sets the exitstatus to the value passed and calls the 
ft_cleanup_loop. it is called when a function which is detecting for
syntax errors findes one.*/
void	es_cul(t_common *c, int es)
{
	c->exitstatus = es;
	ft_cleanup_loop(c);
}

int	analysis_block_redir(t_common *c, char *input, int i, int count)
{
	int	k;
	int	status;

	k = 0;
	status = 0;
	i = skip_whitespace(input, k, 0);
	if ((count == 1 && input[i] == '\0') || (count == 2
			&& input[i] == '\0') || (count == 3 && input[k] == '<'))
		status = ft_putstr_fd(MESSAGE7, 2);
	if (count == 3 && input[k] == '>')
		status = ft_putstr_fd(MESSAGE6, 2);
	if (count == 4 && input[k] == '>')
		status = ft_putstr_fd(MESSAGE4, 2);
	if (count >= 5 && input[k] == '>')
		status = ft_putstr_fd(MESSAGE2, 2);
	if (count == 4 && input[k] == '<')
		status = ft_putstr_fd(MESSAGE5, 2);
	if (count == 5 && input[k] == '<')
		status = ft_putstr_fd(MESSAGE3, 2);
	if (count >= 6 && input[k] == '<')
		status = ft_putstr_fd(MESSAGE1, 2);
	if (status)
		return (es_cul(c, 2), 1);
	return (0);
}

/*This function checks for multiple an wrong usage of < and >. it calls the
analysis_block_redir to print out the correct error message.*/
int	check_block_redir(t_common *c, char *input)
{
	int		i;
	int		count;
	char	red;

	red = '\0';
	count = 0;
	i = skip_whitespace(input, 0, 0);
	if (input[i] == '<' || input[i] == '>')
	{
		red = input[i];
		while (input[i] == red)
		{
			count++;
			i++;
		}
		i = skip_whitespace(input, i, 0);
		if (analysis_block_redir(c, input, i, count))
			return (1);
	}
	return (0);
}

/*This function checks for <, <<, >>, > as a last printable character and
exits in case it finds one.*/
int	check_last_redir(t_common *c, char *input)
{
	int	i;

	i = ft_strlen(input);
	i = skip_whitespace(input, i - 1, 1);
	if (input[i] == '<' || input[i] == '>')
	{
		ft_putstr_fd(MESSAGE7, 2);
		return (es_cul(c, 2), 1);
	}
	return (0);
}

/*This function checks for dots as only input*/
int	check_dots(t_common *c, char *input)
{
	int	i;
	int	k;
	int	dots;

	k = 0;
	i = 0;
	dots = 0;
	i = skip_whitespace(input, i, 0);
	k = i;
	if (input[k] == '.')
	{
		while (input[k++] == '.')
			dots++;
		if (dots == 1 && input[i + 1] == ' ')
			ft_putstr_fd(MESSAGE8, 2);
		if (dots == 2 && input[i + 2] != ' ')
			return (0);
		if (dots > 1)
		{
			while (input[i] != ' ' && input[i] != '\0')
				write (2, &input[i++], 1);
			ft_putstr_fd(MESSAGE13, 2);
		}
		return (es_cul(c, 127), 1);
	}
	return (0);
}

/*This function returns an error message if it finds an empty single or 
double quote or quotes which contains only whitespaces.*/
int	check_empty_quotes(t_common *c, char *input)
{
	int		i;
	char	quote;
	int		k;

	quote = '\0';
	k = 0;
	i = 0;
	i = skip_whitespace(input, i, 0);
	if (input[i] == '\'' || input[i] == '\"')
	{
		quote = input[i];
		i++;
		k = i;
	}
	k = skip_whitespace(input, k, 0);
	if (input[k] == quote)
	{
		while (i < k)
			write (2, &input[i++], 1);
		ft_putstr_fd(MESSAGE13, 2);
		return (es_cul(c, 127), 1);
	}
	return (0);
}

/*This is a function which checks for open double or single quotes. If there are
open quotes it will clear and return the prompt (exitstatus 2).*/
int	check_open_quotes(t_common *c, char *result)
{
	int	i;
	int	s_quote;
	int	d_quote;

	i = 0;
	s_quote = 0;
	d_quote = 0;
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
		ft_putstr_fd(MESSAGE12, 2);
		return (es_cul(c, 2), 1);
	}
	return (0);
}

int	check_next(char *input, int i)
{
	int	len;

	len = ft_strlen(input);
	if (i + 1 <= len)
	{
		if (i + 2 <= len && input[i] == '<' && input[i + 1] == '<')
			return (i + 2);
		else if (i + 2 <= len && input[i] == '>' && input[i + 1] == '>')
			return (i + 2);
		else
			return (i + 1);
	}
	return (i + 1);
}

int	check_invalid_redir(t_common *c, char *input)
{
	size_t	i;
	int		status;

	status = 0;
	i = 0;
	i = skip_whitespace(input, i, 0);
	if (i + 1 <= ft_strlen(input) && (input[i] == '<' || input[i] == '>'))
	{
		i = check_next(input, i);
		if (input[i] != ' ')
			return (0);
		i = skip_whitespace(input, i, 0);
		if (input[i] == '>' && input[i + 1] == '>')
			status = ft_putstr_fd(MESSAGE4, 2);
		else if (input[i] == '<' && input[i + 1] == '<')
			status = ft_putstr_fd(MESSAGE3, 2);
		else if (input[i] == '<')
			status = ft_putstr_fd(MESSAGE5, 2);
		else if (input[i] == '>')
			status = ft_putstr_fd(MESSAGE6, 2);
	}
	if (status)
		return (es_cul(c, 2), 1);
	return (0);
}

/*This function calls many function which will check for different syntax errors
in every token. if an error occours it will catch the return value and returns 1
itself.*/
int	error_tree(t_common *c, char *input)
{
	if (check_open_quotes(c, input))
		return (1);
	if (check_empty_quotes(c, input))
		return (1);
	if (check_dots(c, input))
		return (1);
	if (check_invalid_redir(c, input))
		return (1);
	if (check_block_redir(c, input))
		return (1);
	if (check_last_redir(c, input))
		return (1);
	return (0);
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

/*if the last printable character of the input is a pipe ('|') it enters the 
error_lexer function and displays an error message (incl freeing)*/
int	check_pipe_error_last(t_common *c, char *input, int len)
{
	len = skip_whitespace(input, len - 1, 1);
	if (input[len] == '|')
		return (error_lexer(c, 1), 1);
	return (0);
}

/*this function checks if an pipe ('|') related syntax error occurs 
and enters the error_lexer function and displays an error message 
(incl freeing) if this is the case*/
int	check_pipe_error(t_common *c, char *input, int len)
{
	int	i;
	int	pipe;

	i = 0;
	pipe = 0;
	i = skip_whitespace(input, i, 0);
	if (i + 1 < len && input[i] == '|'
		&& input[i + 1] == '|' && !q_status(input, i))
		return (error_lexer(c, 2), -1);
	if (i < len && input[i] == '|' && !q_status(input, i))
		return (error_lexer(c, 1), -1);
	while (input[i])
	{
		if (input[i] == '|' && !q_status(input, i))
		{
			i++;
			pipe = 1;
			i = skip_whitespace(input, i, 0);
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
	if (check_open_quotes(c, input) != 0
		|| check_pipe_error(c, input, len) != 0
		|| check_pipe_error_last(c, input, len) != 0)
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
