/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:31 by caigner           #+#    #+#             */
/*   Updated: 2024/05/11 23:56:57 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_printerror(t_common *c, char **args)
{
	c->exitstatus = 2;
	ft_putstr_fd("minishell: pwd: -", 2);
	ft_putchar_fd(args[1][1], 2);
	ft_putstr_fd(": invalid option\n", 2);
	ft_clean_exit(c, NULL, 1);
}

int	ft_pwd(t_common *c, char **args)
{
	t_env	*tmp;

	if (args[1])
		if (args[1][0] == '-')
			ft_printerror(c, args);
	tmp = c->env;
	while (tmp)
	{
		if (!tmp)
		{
			c->exitstatus = 1;
			return (ft_putstr_fd("minishell: OLDPWD not set\n", 2), 1);
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
