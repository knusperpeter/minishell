/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 20:17:44 by chris             #+#    #+#             */
/*   Updated: 2024/02/21 13:06:49 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

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
	c->tokens = malloc (sizeof(t_list *));
	if (!c->tokens)
		return (perror("Error initializing t_list\n"), 1);
	c->tokens->content = malloc (sizeof(t_token *));
	if (!c->tokens->content)
		return (perror("Error initializing t_token\n"), 1);
	c->cmd_struct = malloc (sizeof(t_list *));
	if (!c->cmd_struct)
		return (perror("Error initializing t_list\n"), 1);
	c->cmd_struct->content = malloc(sizeof(t_cmd_table *));
	if (!c->cmd_struct->content)
		return (perror("Error initializing cmd_struct\n"), 1);
	while (1)
	{
		ft_exec(c);
		ft_lstiter(c->cmd_struct, free_cmd_table);
	//		free_loop_data(c);
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
