/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:47:51 by miheider          #+#    #+#             */
/*   Updated: 2024/05/13 01:16:26 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	error_and_cleanup(t_common *c, int status)
{
	if (status)
	{
		c->exitstatus = 2;
		ft_cleanup_loop(c);
		return (1);
	}
	return (0);
}
