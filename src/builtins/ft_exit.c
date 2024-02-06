/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:50 by caigner           #+#    #+#             */
/*   Updated: 2024/02/06 21:48:13 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	overflows_ll(t_common *c, char *arg)
{
	long long	i;
	int			len;
	char		*s;
	
	len = ft_strlen(arg);
	i = ft_atoll(arg);
	s = ft_lltoa(i);
	if (ft_strncmp(arg, s, len))
	{
		free(s);
		return (1);
	}
	free(s);
	return (0);
}

int	check_arg(t_common *c, char **arg)
{
	int	i;

	i = 0;
	while (arg[1][i])
	{//this does not check for input like "exit +++" or "exit "--123-"" Maybe it would be best to put this whole if statement into a sparte function.
		if ((!ft_isdigit(arg[1][i]) && arg[1][i] != '-' && arg[1][i] != '+'
				&& arg[1][i] != ' ') || overflows_ll(c, arg[1]))
		{
			perror("minishell: exit: ");
			perror(arg[1]);
			perror(": numeric argument required");
			return (-1);
		}
		i++;
	}
	if (arg[2])
	{
		return (perror("minishell: exit: too many arguments"), 0); // don't exit in this case
	}
	else if (arg[1])
		c->exitstatus = ft_atoll(arg[1]) % 256;//check if negative nums might appear
	else
	 	c->exitstatus = 0;
	return (1);
}
//to hand back the exit status from subshell, use waitpid in parent process?
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
	return ;
}
