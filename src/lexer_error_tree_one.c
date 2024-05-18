/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error_tree_one.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 20:08:50 by miheider          #+#    #+#             */
/*   Updated: 2024/05/18 20:49:18 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
int	check_dots(t_common *c, char *input, int i, int k)
{
	int	dots;

	dots = 0;
	i = skip_whitespace(input, i, 0);
	k = i;
	if (input[k] == '.')
	{
		while (input[k++] == '.')
			dots++;
		if (dots == 1 && (input[i + 1] == ' ' || input[i + 1] == '\0'))
			ft_putstr_fd(MESSAGE8, 2);
		else if (dots == 2 && (input[i + 2] != ' ' || input[i + 2] == '\0'))
			return (0);
		else if (dots > 2)
		{
			while (input[i] != ' ' && input[i] != '\0')
				write (2, &input[i++], 1);
			ft_putstr_fd(MESSAGE13, 2);
		}
		else
			return (0);
		return (es_cul(c, 127), 1);
	}
	return (0);
}
