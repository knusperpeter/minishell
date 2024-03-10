/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@42>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 21:22:44 by miheider          #+#    #+#             */
/*   Updated: 2024/03/09 16:54:35 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>

void    ignore_signal(int signum)
{
    signal(signum, SIG_IGN);
}


void signal_cmd_c_ia(int signum)
{
//    (void)info;
//    (void)ucontent;
    write(1, "^C\n", 3);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

void signal_cmd_c_nonia(int signum)
{
//    (void)info;
//    (void)ucontent;
    g_signal = 2;
    write(1, "^C\n", 3);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

void    signal_bs_nonia(int signum)
{
//    (void)info;
//    (void)ucontent;
    g_signal = 3;
    write(1, "\n^\\ Quit (core dumped)\n", 23);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}


void    interactive(void)
{
    struct sigaction    signal;

    signal.sa_flags = 0;
    signal.sa_sigaction = &signal_cmd_c_ia;
    sigemptyset(&signal.sa_mask);
    sigaction(SIGINT, &signal, NULL);

    signal.sa_sigaction = &ignore_signal;
    sigaction(SIGQUIT, &signal, NULL);
}
void    non_interactive(void)
{
    struct sigaction    signal;

    signal.sa_flags = 0;
    signal.sa_sigaction = &signal_cmd_c_nonia;
    sigemptyset(&signal.sa_mask);
    sigaction(SIGINT, &signal, NULL);

    signal.sa_sigaction = &signal_bs_nonia;
    sigaction(SIGQUIT, &signal, NULL);
}
