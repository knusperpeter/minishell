/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 21:22:44 by miheider          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/03/23 16:34:57 by miheider         ###   ########.fr       */
=======
/*   Updated: 2024/03/23 15:08:23 by miheider         ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

<<<<<<< HEAD
void	ignore_signal(int signum, siginfo_t *info, void *ucontent)
{
	(void)info;
	(void)ucontent;

	struct sigaction	sig_ignore;
	sig_ignore.sa_flags = 0;
	sig_ignore.sa_handler = SIG_IGN; // Set action to ignore signal
	sigemptyset(&sig_ignore.sa_mask);
	sigaction(signum, &sig_ignore, NULL); // Set signal action
}

void	signal_cmd_c_ia(int signal, siginfo_t *info, void *ucontent)
{
	(void)info;
	(void)ucontent;
	(void)signal;
	write(1, "^C\n", 3);
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
	write(1, "^C\n", 3);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
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
=======
void ignore_signal(int signum, siginfo_t *info, void *ucontent)
{
    (void)info;
    (void)ucontent;

    struct sigaction sa_ignore;
    sa_ignore.sa_flags = 0;
    sa_ignore.sa_handler = SIG_IGN; // Set action to ignore signal
    sigemptyset(&sa_ignore.sa_mask);

    sigaction(signum, &sa_ignore, NULL); // Set signal action
}


void signal_cmd_c_ia(int signal, siginfo_t *info, void *ucontent)
{
    (void)info;
    (void)ucontent;
    (void)signal;
    write(1, "^C\n", 3);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

void signal_cmd_c_nonia(int signal, siginfo_t *info, void *ucontent)
{
    (void)info;
    (void)ucontent;
    (void)signal;
//    g_signal = 2;
    write(1, "^C\n", 3);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

void    signal_bs_nonia(int signal, siginfo_t *info, void *ucontent)
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


void interactive(void)
{
    struct sigaction sig_int;
    struct sigaction sig_quit;

    sig_int.sa_flags = 0;
    sig_int.sa_sigaction = &signal_cmd_c_ia;
    sigemptyset(&sig_int.sa_mask);
    sigaction(SIGINT, &sig_int, NULL);

    sig_quit.sa_flags = 0;
    sig_quit.sa_sigaction = &ignore_signal;
    sigemptyset(&sig_quit.sa_mask);
    sigaction(SIGQUIT, &sig_quit, NULL);
>>>>>>> main
}

void	non_interactive(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sig_int.sa_flags = 0;
	sig_int.sa_sigaction = &signal_cmd_c_nonia;
	sigemptyset(&sig_int.sa_mask);
	sigaction(SIGINT, &sig_int, NULL);
	sig_quit.sa_flags = 0;
	sig_quit.sa_sigaction = &signal_bs_nonia;
	sigemptyset(&sig_quit.sa_mask);
	sigaction(SIGQUIT, &sig_quit, NULL);
}
