/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 21:22:44 by miheider          #+#    #+#             */
/*   Updated: 2024/05/11 21:57:35 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	cmd_c_ia(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		(void)sig;
	}
	else if (sig == SIGQUIT)
		g_signal = 0;
}

void	cmd_c_nonia(int sig)
{
	if (sig == SIGINT)
	{
		(void) sig;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
//		rl_redisplay();
		(void)sig;
	}
	else if (sig == SIGQUIT)
		g_signal = 0;
}

void	cmd_bs(int sig)
{
	if (sig == SIGQUIT)
	{
		g_signal = SIGQUIT;
		write (1, "Quit (core dumped)\n", 19);
		(void) sig;
	}
	else if (sig == SIGINT)
		g_signal = 0;
}


void	cmd_c_here(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = SIGINT;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		ioctl(1, TIOCSTI, "\n");
		(void)sig;
	}
	else if (sig == SIGQUIT)
		g_signal = 0;
}

void	interactive(t_common *c)
{
	if (g_signal != 0)
	{
		c->exitstatus = g_signal + 128;
		g_signal = 0;
	}
	signal(SIGINT, cmd_c_ia);
	signal(SIGQUIT, SIG_IGN);
}

void	non_interactive(t_common *c)
{
	if (g_signal != 0)
	{
		c->exitstatus = g_signal + 128;
		g_signal = 0;	
	}
	signal(SIGINT, cmd_c_nonia);
	signal(SIGQUIT, cmd_bs);
}

void	interactive_here(t_common *c)
{
	if (g_signal != 0)
	{
		c->exitstatus = g_signal + 128;
		g_signal = 0;	
	}
	signal(SIGINT, cmd_c_here);
	signal(SIGQUIT, SIG_IGN);
}