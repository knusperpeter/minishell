/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 20:50:22 by miheider          #+#    #+#             */
/*   Updated: 2024/05/18 20:50:37 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
