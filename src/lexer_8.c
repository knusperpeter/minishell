/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:55:58 by miheider          #+#    #+#             */
/*   Updated: 2024/05/13 01:19:41 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_for_open_quotes(int *s_quote, int *d_quote, int *i)
{
	*s_quote = 0;
	*i = 0;
	*d_quote = 0;
}

int	open_quotes(t_common *c, char *result)
{
	int	i;
	int	s_quote;
	int	d_quote;

	init_for_open_quotes(&s_quote, &d_quote, &i);
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
		ft_putstr_fd("❌ minishell: syntax error dquote\n", 2);
		c->exitstatus = 2;
		ft_cleanup_loop(c);
		return (1);
	}
	return (0);
}

int	check_sq(t_common *c, char *result, int k)
{
	int	sq;

	sq = 0;
	while (result[k])
	{
		if (result[k] == '\'')
		{
			while (result[k] == '\'')
			{
				sq++;
				k++;
			}
			if (result[k] == ' ' && sq % 2 == 0)
			{
				ft_putstr_fd("❌ minishell: Command '' not found.\n", 2);
				c->exitstatus = 127;
				ft_cleanup_loop(c);
				return (1);
			}
		}
		else
			return (0);
		k++;
	}
	return (0);
}

/* Function to handle the remaining checks and validation */
int	check_following(t_common *c, char *result, int k, char fir)
{
	int	i;
	int	j;

	i = k;
	j = 0;
	while (result[i] && i < k + 6)
	{
		if (result[i] == fir)
			j++;
		else
			break ;
		i++;
	}
	if (check_that(c, &result[k], k) || (j > 0 && (check_this(c, &result[k], j)
				|| check_dot(c, &result[0], k, j)
				|| check_quotes(c, &result[k], k)
				|| check_again(c, &result[k], k, fir)
				|| check_more(c, &result[0], k, fir))))
		return (1);
	return (0);
}

void	init_initial_checks(int *i, int *k, int*len, char *result)
{
	*i = 0;
	*k = 0;
	*len = ft_strlen(result);
}
