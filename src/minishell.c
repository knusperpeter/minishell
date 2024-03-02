/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 20:17:44 by chris             #+#    #+#             */
/*   Updated: 2024/03/02 16:11:07 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*prompt(void)
{
	char	*line;

	line = readline("minishell🔮: 🚬🦦❯ "); // check rl_redisplay
	if (ft_strlen(line) > 0)
		add_history(line);
	return (line);
}

void	init_loop_data(t_common *c){
	c->raw_prompt = NULL;
	c->tokens = NULL;
	c->cmd_struct = NULL;
	c->envp = NULL;
	*c->old_pipe.read_fd = -1;
	*c->old_pipe.write_fd = -1;
	*c->new_pipe.read_fd = -1;
	*c->new_pipe.write_fd = -1;
}

int	ft_loop(t_common *c)
{
	init_loop_data(c);
	c->raw_prompt = prompt();
	if (c->raw_prompt[0])
	{
		ft_parsing(c);
		ft_execute(c);
		ft_cleanup_loop(c);
		//ft_execute_builtins(c->cmd_struct->content, c);
	}
	return (0);
}

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

int	init_loop(t_common *c)
{
//protect that shit, also maybe make a function, that will init t_list and subs
	c->tokens = ft_lstnew(malloc (sizeof(t_token *)));
	if (!c->tokens || !c->tokens->content)
		return (perror("Error initializing tokens\n"), 1);
	while (1)
	{
		ft_loop(c);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_common	c;
	// Validate size before allocation. The size variable
	// should be validated or bounded checked before allocating memory to avoid potential integer overflows.
	(void)		ac;
	(void)		av;
	
	init_minishell(&c, envp);
	init_loop(&c);
	free_all(&c, c.cmd_struct->content);
	return (0);
}
