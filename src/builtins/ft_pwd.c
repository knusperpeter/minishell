/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:31 by caigner           #+#    #+#             */
/*   Updated: 2024/05/06 17:05:32 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(t_common *c)
{
	t_env	*tmp;

	tmp = c->env;
	while (tmp)
	{
		if (!tmp)
		{
			c->exitstatus = 1;
			return(ft_putstr_fd("minishell: OLDPWD not set\n", 2), 1);
		}
		if (!strncmp(tmp->variable, "PWD", 4))
		{
			c->exitstatus = 0;
			return (printf("%s\n", tmp->value), 0);
		}
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}
