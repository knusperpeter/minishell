/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_prep_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 20:00:11 by miheider          #+#    #+#             */
/*   Updated: 2024/05/19 13:35:31 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	update_counts(int *i, int *cc, char *input, int num)
{
	if (*i > 0 && input[*i - 1] != ' ')
		counting_up(i, cc, (num / 1000), (num % 1000) / 100);
	else
		counting_up(i, cc, (num % 100) / 10, num % 10);
}

void	process_checks(char *input, int *i, int *cc, int len)
{
	if (check_double(input, *i, len) == 1)
		(*cc)++;
	if (check_double(input, *i, len) == 2)
		(*cc)--;
	if (check_the_char(input, *i, len) == 1)
		update_counts(i, cc, input, 3433);
	if (check_the_char(input, *i, len) == 2)
		update_counts(i, cc, input, 2434);
	if (check_the_char(input, *i, len) == 3)
		update_counts(i, cc, input, 2322);
	if (check_the_char(input, *i, len) == 4)
		update_counts(i, cc, input, 1323);
}

void	init_prep_input(int *i, int *cc, int *len, char *input)
{
	*i = 0;
	*cc = 0;
	*len = ft_strlen(input);
}

int	check_the_end(char *input, int i, int *cc)
{
	if (input[i] == '\0')
	{
		if (input[i - 1] == ' ')
			(*cc)--;
		return (1);
	}
	return (0);
}

char	**prep_input(t_common *c, char *input)
{
	int	i;
	int	cc;
	int	len;

	init_prep_input(&i, &cc, &len, input);
	i = skip_whitespace(input, i, 0);
	while (input[i])
	{
		if (q_status(input, i) == 0)
		{
			if (i < len && (input[i] == ' '))
			{
				counting_up(&i, &cc, 1, 1);
				i = skip_whitespace(input, i, 0);
				if (check_the_end(input, i, &cc) == 1)
					break ;
				continue ;
			}
			process_checks(input, &i, &cc, len);
			i = skip_whitespace(input, i, 0);
		}
		counting_up(&i, &cc, 1, 1);
	}
	return (set_up_array(c, cc + 1, input));
}
