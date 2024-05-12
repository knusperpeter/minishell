/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 10:53:28 by chris             #+#    #+#             */
/*   Updated: 2024/05/12 17:15:00 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_printerrno(char *s)
{
	ft_putstr_fd("minishell: ", 2);
	if (s)
		ft_putstr_fd(s, 2);
	ft_putchar_fd('\n', 2);
}

void	handle_quote_state(t_common *common, char c)
{
	if (c == '\'' && !common->open_double_quotes)
	{
		if (common->open_single_quotes)
			common->open_single_quotes = 0;
		else
			common->open_single_quotes = 1;
	}
	else if (c == '\"' && !common->open_single_quotes)
	{
		if (common->open_double_quotes)
			common->open_double_quotes = 0;
		else
			common->open_double_quotes = 1;
	}
}

int	get_env_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}