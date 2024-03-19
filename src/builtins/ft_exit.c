/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:50 by caigner           #+#    #+#             */
/*   Updated: 2024/03/19 23:50:52 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	overflows_ll(t_common *c, char *arg)
{
	long long	i;
	int			len;
	char		*s;

	(void) c;
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

int	is_sign(char c)
{
	if (c == '-' || c == '+')
		return (1);
	return (0);
}

int	valid_num(char *arg)
{
	int	i;

	if (!arg || !arg[0])
		return (0);
	i = 0;
	while (arg[i] == ' ')
		i++;
	if ((int)ft_strlen(arg) > i && is_sign(arg[i])) //dannach muss eine zahl kommen, wenn nach der zahl was anderes als leerzeichen kommt, dann ist es falsch. same error
		i++;
	while (arg[i])
	{
		if (ft_isdigit(arg[i]))
		{
			while (arg[i] == ' ')
				i++;
			if (arg[i] == '\0')
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_numeric_arg(t_common *c, char **arg)
{
	int	i;
	
	if (!arg[1] || !arg[1][0])
		return (1);
	i = 0;
	
	if (!valid_num(arg[1]) || overflows_ll(c, arg[1]))
	{
		ft_putstr_fd("❌ minishell: exit: ", 2);
		ft_putstr_fd(arg[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (c->exitstatus = 2, 0);
	}
	return (1);
}

// this does not check for input like "exit +++" or "exit "--123-"" Maybe it
// would be best to put this whole if statement into a sparte function.
int	check_arg(t_common *c, char **arg)
{
	if (!arg[1])
		return (c->exitstatus = 0, -1);
	if (!ft_numeric_arg(c, arg))
		return (2);
	if (arg[2])
	{
		return (ft_putstr_fd("❌ minishell: exit: too many arguments\n", 1), 1);// don't exit in this case
	}
	else if (arg[1])
		c->exitstatus = ft_atoll(arg[1]) % 256;//check if negative nums might appear
	return (-1);
}

//to hand back the exit status from subshell, use waitpid in parent process?
void	ft_exit(t_common *c, char **cmd)
{
	printf("exit\n");
	if (cmd)
		if (cmd[1])
			if (check_arg(c, cmd) == 1)
				return ;
	ft_clean_exit(c, NULL);
}

/* 
EXIT STATUS
The  exit  status  of an executed command is the value returned by the waitpid
system call or equivalent function.  Exit statuses fall  between  0  and  255,
though,  as  explained  below,  the  shell may use values above 125 specially.
Exit statuses from shell builtins and compound commands are  also  limited  to
this range.  Under certain circumstances, the shell will use special values to
indicate specific failure modes.

For the shell's purposes, a command which exits with a zero  exit  status  has
succeeded.   An exit status of zero indicates success.  A non-zero exit status
indicates failure.  When a command terminates on a fatal signal N,  bash  uses
the value of 128+N as the exit status.

If  a  command is not found, the child process created to execute it returns a
status of 127.  If a command is found but is not executable, the return status
is 126.

If  a  command  fails because of an error during expansion or redirection, the
exit status is greater than zero.

Shell builtin commands return a status of 0 (true) if successful, and non-zero
(false)  if  an  error occurs while they execute.  All builtins return an exit
status of 2 to indicate incorrect usage, generally invalid options or  missing
arguments.

Bash  itself  returns  the  exit status of the last command executed, unless a
syntax error occurs, in which case it exits with a non-zero value.   See  also
the exit builtin command below.
 */