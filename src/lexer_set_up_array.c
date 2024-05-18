/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_set_up_array.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 20:24:45 by miheider          #+#    #+#             */
/*   Updated: 2024/05/18 21:21:45 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*this function is writing the characters of the token into the allocated
memmory. it skips multiple spaces and inserts spaces if needed. the new_string
which is passed to the tokenize_input function.*/
int	handle_whitespace(char *input, int k, char *new_string, int *j)
{
	k = skip_whitespace(input, k, 0);
	if (k > 0 && *j > 0 && input[k - 1] == ' ' && input[k] != '\0'
		&& new_string[*j - 1] != ' ')
	{
		if (*j != 0)
			new_string[(*j)++] = ' ';
	}
	return (k);
}

void	handle_special_characters(char *input, int *k, char *new_string, int *j)
{
	if (*k > 0 && check_space_before(input, *k) == 1
		&& new_string[*j - 1] != ' ')
		new_string[(*j)++] = ' ';
	new_string[(*j)++] = input[*k];
	if (input[*k + 1] == input[*k])
	{
		new_string[(*j)++] = input[*k];
		(*k)++;
	}
	if (check_space_after(input, *k) == 1 && input[*k + 1] != '\0'
		&& input[*k + 1] != ' ')
		new_string[(*j)++] = ' ';
}

void	process_character(char *input, int *k, char *new_string, int *j)
{
	if (q_status(input, *k) == 0)
	{
		*k = handle_whitespace(input, *k, new_string, j);
		if (input[*k] == '<' || input[*k] == '>')
			handle_special_characters(input, k, new_string, j);
		else
			new_string[(*j)++] = input[*k];
	}
	else
		new_string[(*j)++] = input[*k];
}

void	process_input(char *input, char *new_string, int cc)
{
	int	j;
	int	k;

	j = 0;
	k = skip_whitespace(input, 0, 0);
	while (input[k])
	{
		if (j >= cc - 1)
			break ;
		process_character(input, &k, new_string, &j);
		k++;
	}
	new_string[j] = '\0';
}

char	**set_up_array(t_common *c, int cc, char *input)
{
	char	*new_string;

	new_string = ft_protect(c, (char *)malloc(sizeof(char) * (cc)), 0, 0);
	process_input(input, new_string, cc);
	return (tokenize_input(new_string));
}
