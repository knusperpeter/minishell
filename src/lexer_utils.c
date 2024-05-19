/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 20:28:34 by miheider          #+#    #+#             */
/*   Updated: 2024/05/19 13:36:18 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
