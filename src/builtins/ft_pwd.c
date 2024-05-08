/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:31 by caigner           #+#    #+#             */
/*   Updated: 2024/05/08 17:41:09 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(t_common *c)
{
	t_env	*tmp;

//	invalid option error message missing
	tmp = c->env;
	while (tmp)
	{
		if (!tmp)
		{
			c->exitstatus = 1;
			return(ft_putstr_fd("minishell: OLDPWD not set\n", 2), 1);
		}
		if (!ft_strncmp(tmp->variable, "PWD", 4))
		{
			c->exitstatus = 0;
			return (printf("%s\n", tmp->value), 0);
		}
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}
