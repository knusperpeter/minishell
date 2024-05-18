/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:50 by caigner           #+#    #+#             */
/*   Updated: 2024/05/18 16:01:13 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_sign(char c)
{
	if (c == '-' || c == '+')
		return (1);
	return (0);
}

char	*get_llstr(t_common *c, char *arg, int i, int num_len)
{
	char	*str;
	int		j;
	int		minus;

	minus = 0;
	j = 0;
	while (arg[j] && ft_strchr(WHITESPACE, arg[j]))
		j++;
	if (is_sign(arg[j]))
	{
		if (arg[j] == '-')
			minus = 1;
		j++;
	}
	str = ft_protect(c, malloc(sizeof(char) * (num_len + minus + 1)), 0, 0);
	if (str == NULL)
		return (NULL);
	j = 0;
	if (minus)
		str[j++] = '-';
	while (arg[i] && ft_isdigit(arg[i]))
		str[j++] = arg[i++];
	str[j] = '\0';
	return (str);
}

int	overflows_ll(t_common *c, char *arg)
{
	int		i;
	char	*num;
	char	*s;
	int		num_len;

	i = 0;
	num_len = 0;
	while (arg[i] && ft_strchr(WHITESPACE, arg[i]))
		i++;
	if (is_sign(arg[i]))
		i++;
	while (arg[i] == '0')
		i++;
	num_len = 0;
	while (ft_isdigit(arg[i + num_len]))
		num_len++;
	s = get_llstr(c, arg, i, num_len);
	num = ft_protect(c, ft_lltoa(ft_atoll(s)), s, 0);
	if (ft_strncmp(s, num, num_len))
		return (free(s), free(num), 1);
	return (free(s), free(num), 0);
}

int	valid_num(char *arg)
{
	int	i;

	if (!*arg)
		return (0);
	i = 0;
	while (arg[i] && ft_strchr(WHITESPACE, arg[i]))
		i++;
	if (is_sign(arg[i]))
		i++;
	if (!ft_isdigit(arg[i]))
		return (0);
	while (arg[i])
	{
		if (ft_isdigit(arg[i]))
			i++;
		else
			break ;
	}
	while (arg[i] && ft_strchr(WHITESPACE, arg[i]))
		i++;
	if (arg[i] != '\0')
		return (0);
	return (1);
}

//to hand back the exit status from subshell, use waitpid in parent process?
void	ft_exit(t_common *c, char **cmd)
{
	if (c->cmd_count == 1)
		ft_putstr_fd("exit\n", 2);
	if (cmd[1])
	{
		if (!cmd[1])
			c->exitstatus = 0;
		if (!valid_num(cmd[1]) || overflows_ll(c, cmd[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(cmd[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			c->exitstatus = 2;
		}
		if (cmd[2])
		{
			c->exitstatus = 1;
			ft_putstr_fd("minishell: exit: too many args\n", 2);
			return ;
		}
		else if (cmd[1])
			c->exitstatus = ft_atoll(cmd[1]) % 256;
	}
	ft_clean_exit(c, NULL, 1);
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