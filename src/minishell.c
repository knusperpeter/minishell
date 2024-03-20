/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 20:17:44 by chris             #+#    #+#             */
/*   Updated: 2024/03/20 15:05:25 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * Function: prompt
 * Description: Displays a prompt and reads a line of input from the user.
 * Returns: The line of input from the user.
 */
char	*prompt(t_common *c)
{
	char	*line;

	line = readline("minishell🔮: 🚬🦦❯ "); // check rl_redisplay
	if (line == NULL)
		ft_exit(c, NULL);
	if (line && *line)
		add_history(line);
	return (line);
}

void	init_pipe(t_pipe *pipe)
{
	pipe->pipes[0] = -1;
	pipe->pipes[1] = -1;
	pipe->read_fd = &pipe->pipes[0];
	pipe->read_fd = &pipe->pipes[1];
}

/**
 * Function: init_loop_data
 * Description: Initializes the data for the main loop of the shell.
 * Parameter: c - The common structure containing the shell data.
 */
void	init_loop_data(t_common *c){
	c->raw_prompt = NULL;
	c->tokens = NULL;
	c->cmd_struct = NULL;
	c->envp = NULL;
	c->exitstatus_str = NULL;
	init_pipe(&c->new_pipe);
	init_pipe(&c->old_pipe);
}

/**
 * Function: ft_loop
 * Description: The main loop of the shell. It reads input, parses it, executes it, and cleans up.
 * Parameter: c - The common structure containing the shell data.
 * Returns: 0 when the loop ends.
 */
int	ft_loop(t_common *c)
{
	while (1)
	{
		init_loop_data(c);
	//	interactive();
		c->raw_prompt = prompt(c);
//		ft_putstr_fd(c->raw_prompt, 1);
//		if (!c->raw_prompt[0])
//			break ;
		if (c->raw_prompt[0])
		{
			//non_interactive();
			ft_parsing(c);
			ft_execute(c);
			ft_cleanup_loop(c);
		}
	}
	return (0);
}

/**
 * Function: init_minishell
 * Description: Initializes the shell.
 * Parameters:
 * - c: The common structure containing the shell data.
 * - envp: The environment variables.
 */
void	init_minishell(t_common *c, char **envp)
{
	ft_memset(c, 0, sizeof(t_common));
	if(envp)
	{
		if (!dup_env(c, envp))
			return (perror("Error initializing environment\n"));
	}
	else
		return (perror("Error initializing environment\n"));
}

/**
 * Function: main
 * Description: The entry point of the program. It initializes the shell, runs the main loop, and cleans up when done.
 * Parameters:
 * - ac: The number of command-line arguments.
 * - av: The command-line arguments.
 * - envp: The environment variables.
 * Returns: 0 when the program ends.
 */
int	main(int ac, char **av, char **envp)
{
	t_common	c;
	// Validate size before allocation. The size variable
	// should be validated or bounded checked before allocating memory to avoid potential integer overflows.
	(void)		ac;
	(void)		av;

	init_minishell(&c, envp);
	ft_loop(&c);
	free_all(&c);
	return (0);
}
