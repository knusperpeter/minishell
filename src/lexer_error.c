/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:16:46 by miheider          #+#    #+#             */
/*   Updated: 2024/05/18 20:48:33 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	error_lexer(t_common *c, int i)
{
	int	status;

	status = 0;
	if (i == 1)
		status = ft_putstr_fd(MESSAGE10, 2);
	else
		status = ft_putstr_fd(MESSAGE11, 2);
	if (status)
	{
		c->exitstatus = 2;
		ft_cleanup_loop(c);
		return (1);
	}
	return (0);
}
