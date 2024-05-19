/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 19:37:00 by caigner           #+#    #+#             */
/*   Updated: 2024/05/19 13:46:05 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_signal = 0;

void	prompt(t_common *c)
{
	c->raw_prompt = readline("minishell> ");
	if (c->raw_prompt && c->raw_prompt[0] != '\0')
		add_history(c->raw_prompt);
}

void	init_loop_data(t_common *c)
{
	c->raw_prompt = NULL;
	c->tokens = NULL;
	c->cmd_struct = NULL;
	c->envp = NULL;
	c->subshell_level = 0;
	c->old_pipe = 0;
	c->heredoc_counter = 0;
}

int	ft_loop(t_common *c)
{
	c->exitstatus = 0;
	while (1)
	{
		init_loop_data(c);
		interactive(c);
		prompt(c);
		non_interactive(c);
		if (!c->raw_prompt)
			break ;
		if (c->raw_prompt[0])
		{
			if (ft_parsing(c) != EXIT_FAILURE)
				ft_execute(c);
			else
				c->exitstatus = 130;
			ft_cleanup_loop(c);
		}
	}
	return (0);
}

void	init_minishell(t_common *c, char **envp)
{
	ft_memset(c, 0, sizeof(t_common));
	if (envp)
	{
		if (!dup_env(c, envp))
			return (perror("Error initializing environment\n"));
	}
	else
		return (perror("Error initializing environment\n"));
}

int	main(int ac, char **av, char **envp)
{
	t_common	c;

	(void) ac;
	(void) av;
	if (!envp[0] || !envp)
		return (1);
	init_minishell(&c, envp);
	ft_loop(&c);
	free_all(&c, 0);
	return (0);
}
