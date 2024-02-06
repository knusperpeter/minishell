/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:50 by caigner           #+#    #+#             */
/*   Updated: 2024/02/06 20:52:09 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *skipspace(char *s)
{
	while (s && *s == ' ')
	{
		s++;
	}
	return (s);
}

int	check_arg(t_common *c, char **arg)
{
	int	i;

	i = 0;
	while (arg[1][i])
	{
		arg[1] = skipspace(arg[1]);
		if (!ft_isdigit(arg[1][i++]) && arg[1][i] != '-' && arg[1][i] != '+')
		{
			perror("minishell: exit: ");
			perror(arg[1]);
			perror(": numeric argument required");
			return (-1);
		}
	}
	if (arg[2])
	{
		return (perror("minishell: exit: too many arguments"), 0); // don't exit in this case
	}
	else if (arg[1])
		c->exitstatus = ft_atoi(arg[1]);
	else
	 	c->exitstatus = 0;
	return (1);
}
//to hand back the exit status from subshell, use waitpid in parent process
void	ft_exit(t_common *c, char **cmd)
{
	c->exitstatus = 1;
	printf("exit\n");
	if (cmd[1])
	{
		if (!check_arg(c, cmd))
		{
			return ;
		}
	}
	//status %= 256;
	
	return ;
}
