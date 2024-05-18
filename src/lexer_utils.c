/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 20:28:34 by miheider          #+#    #+#             */
/*   Updated: 2024/05/18 20:28:51 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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