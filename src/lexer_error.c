/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:16:46 by miheider          #+#    #+#             */
/*   Updated: 2024/05/09 14:19:06 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	error_lexer(t_common *c, char *s, int i)
{
	int status;

	status = 0;
	if (*s == '|')
	{
		if (*s == '|' && (i == 2 || i == 3))
			status = ft_putstr_fd("❌ minishell: syntax error near unexpected token `|'\n", 2);
		else if (*s == '|' && i > 3)
			status = ft_putstr_fd("❌ minishell: syntax error near unexpected token `||'\n", 2);
		if (status)
		{
			c->exitstatus = 2;
			ft_cleanup_loop(c);
			return (1);
		}
	}	
	return (0);
}
