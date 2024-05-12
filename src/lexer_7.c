/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:53:55 by miheider          #+#    #+#             */
/*   Updated: 2024/05/12 21:54:13 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_more_cont(t_common *c, char *result, int k, int status)
{
	int	len;

	status = ft_putstr_fd(MESSAGE9, 2);
	if (result[k] == '>')
		write(2, &result[k + 3], 1);
	else
		write(2, "<", 1);
	len = ft_strlen(result);
	if (len >= 5 && (result[k + 4] == '<' || result[k + 4] == '>'))
	{
		if (result[k + 4] == '>')
			write(2, &result[k], 1);
		else
			write(2, "<", 1);
	}
	write(2, "'", 1);
	return (error_and_cleanup(c, status));
}

int	check_more(t_common *c, char *result, int k, char fir)
{
	int	len;
	int	status;

	status = 0;
	len = ft_strlen(result);
	if (!(len >= 4 && result[k] == result[k + 1] && result[k] == fir
			&& result[k + 2] == ' ' && (result[k + 3] == '<'
				|| result[k + 3] == '>')))
		return (0);
	if (result[k + 3] == '<' || result[k + 3] == '>')
		return (check_more_cont(c, result, k, status));
	else
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
	{
		c->exitstatus = 2;
		ft_cleanup_loop(c);
		return (1);
	}
	return (0);
}

int	check_condition_and_error(char *result, int *i)
{
	int	status;

	status = 0;
	while (result[*i] == ' ' || (result[*i] >= 9 && result[*i] <= 13))
		(*i)++;
	if (result[*i] == '\0' || (result[*i] == ' '
			|| (result[*i] >= 9 && result[*i] <= 13)))
	{
		status = ft_putstr_fd(MESSAGE7, 2);
		return (status);
	}
	if (result[*i] == '>')
	{
		(*i)++;
		while (result[*i] == ' ' || (result[*i] >= 9 && result[*i] <= 13))
			(*i)++;
		if (result[*i] == '\0' || (result[*i] == ' '
				|| (result[*i] >= 9 && result[*i] <= 13)))
		{
			status = ft_putstr_fd(MESSAGE7, 2);
			return (status);
		}
	}
	return (status);
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
			status = check_condition_and_error(result, &i);
			if (status)
				break ;
		}
		i++;
	}
	return (error_and_cleanup(c, status));
}