/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:51:12 by miheider          #+#    #+#             */
/*   Updated: 2024/05/13 01:12:53 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		error_and_cleanup(c, status);
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
			return (error_and_cleanup(c, 1));
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
			c->exitstatus = 127;
			ft_cleanup_loop(c);
			return (1);
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
	c->exitstatus = 127;
	ft_putstr_fd (" :command not found\n", 2);
	ft_cleanup_loop(c);
	return (1);
}

int	condition_and_error(t_common *c, char *result, int k, int status)
{
	status = ft_putstr_fd(MESSAGE9, 2);
	if (result[k] == '>')
		write(2, &result[k], 1);
	else
		write(2, "<", 1);
	if (result[k + 1] == '<' || result[k + 1] == '>')
	{
		if (result[k + 1] == '>')
			write(2, &result[k], 1);
		else
			write(2, "<", 1);
	}
	write(2, "'", 1);
	return (error_and_cleanup(c, status));
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
	{
		return (0);
	}
	if (result[k] == '<' || result[k] == '>')
		return (condition_and_error(c, result, k, status));
	else
	{
		return (0);
	}
}
