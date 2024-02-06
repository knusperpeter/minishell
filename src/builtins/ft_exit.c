/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:50 by caigner           #+#    #+#             */
/*   Updated: 2024/02/06 19:32:19 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_arg(char **arg)
{
	int	i;

	i = 0;
	while (arg[0][i])
	{
		if (!ft_isdigit(arg[0][i++]))
		{
			perror("minishell: exit: ");
			perror(arg[0]);
			perror(": numeric argument required");
			return (-1);
		}
	}
	if (arg[1])
	{
		return (perror("minishell: exit: too many arguments"), 0); // don't exit in this case
	}
	return (1);
}
//to hand back the exit status from subshell, use waitpid in parent process
void	ft_exit(char **arg)
{
	int	i;

	i = 0;
	printf("exit\n");
	if (arg[0])
	{
		if (!check_arg(arg))
		{
			return ;
		}
	}
	
	//status %= 256;
	
	return ;
}
