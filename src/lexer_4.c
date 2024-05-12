/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:45:16 by miheider          #+#    #+#             */
/*   Updated: 2024/05/13 01:31:42 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_non_special_character(int *i, int *j, char *input, char *new_string)
{
	if ((input[*i] && check_char(&input[*i]) == 0
			&& check_char(&input[*(i) - 1]) == 1)
		|| (check_char(&input[*i]) == 1 && check_char(&input[*(i) - 1]) == 0
			&& !ft_strchr(WHITESPACE, input[(*i) - 1]))
		|| (check_char(&input[*i]) == 1 && check_char(&input[*(i) - 1]) == 1
			&& input[*i] != input[*(i) - 1]))
	{
		new_string[(*j)++] = ' ';
	}
	new_string[(*j)++] = input[(*i)++];
	return (10);
}

int	no_space_array(int *i, int *j, char *input, char *new_string)
{
	if (input[*i] && check_char(&input[*i]) == 2)
	{
		return (process_special_character(i, j, input, new_string));
	}
	if (*i > 0)
	{
		return (handle_non_special_character(i, j, input, new_string));
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
