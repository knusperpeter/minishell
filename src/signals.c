/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 21:22:44 by miheider          #+#    #+#             */
/*   Updated: 2024/04/22 16:44:58 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ignore_signal(int signum, siginfo_t *info, void *ucontent)
{
	(void)signum;
	(void)info;
	(void)ucontent;
}

void	signal_cmd_c_ia(int signal, siginfo_t *info, void *ucontent)
{
	(void)info;
	(void)ucontent;
	(void)signal;
//	printf("Das funkt jetzt!");
	write(1, "\n", 2);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	signal_cmd_c_nonia(int signal, siginfo_t *info, void *ucontent)
{
	(void)info;
	(void)ucontent;
	(void)signal;
//    g_signal = 2;
	write(1, "\n", 2);
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	signal_bs_nonia(int signal, siginfo_t *info, void *ucontent)
{
	(void)info;
	(void)ucontent;
	(void)signal;
//    g_signal = 3;
	write(1, "\n^\\ Quit (core dumped)\n", 23);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	interactive(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sig_int.sa_flags = 0;
	sig_int.sa_sigaction = &signal_cmd_c_ia;
	sigemptyset(&sig_int.sa_mask);
	sigaction(SIGINT, &sig_int, NULL);
	sig_quit.sa_flags = 0;
	sig_quit.sa_sigaction = &ignore_signal;
	sigemptyset(&sig_quit.sa_mask);
	sigaction(SIGQUIT, &sig_quit, NULL);
}

void	non_interactive(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sig_quit.sa_flags = 0;
	sig_quit.sa_sigaction = &signal_bs_nonia;
	sigemptyset(&sig_quit.sa_mask);
	sigaction(SIGQUIT, &sig_quit, NULL);
	sig_int.sa_flags = 0;
	sig_int.sa_sigaction = &signal_cmd_c_nonia;
	sigemptyset(&sig_int.sa_mask);
	sigaction(SIGINT, &sig_int, NULL);
}
