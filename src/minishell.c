/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 20:17:44 by chris             #+#    #+#             */
/*   Updated: 2024/02/17 20:32:19 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	c->tokenslist = malloc(sizeof(t_node *));
	if (!c->tokenslist)
		return (perror("Error initializing tokenslist\n"), 1);
	while (1)
	{
		ft_exec(c);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_common	c;
	//Validate size before allocation. The size variable
	// should be validated or bounded checked before allocating memory to avoid potential integer overflows.
	(void)		ac;
	(void)		av;
	
	init_minishell(&c, envp);
	init_loop(&c);
	free_all(&c);
	return (0);
}
