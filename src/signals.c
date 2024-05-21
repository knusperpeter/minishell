/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 21:22:44 by miheider          #+#    #+#             */
/*   Updated: 2024/05/21 19:56:48 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	cmd_c_ia(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = SIGINT;
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
		g_signal = SIGINT;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
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
//		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
//		rl_redisplay();
		ioctl(1, TIOCSTI, "\n");
		(void)sig;
	}
	else if (sig == SIGQUIT)
		g_signal = 0;
}
