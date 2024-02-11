/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 02:56:18 by caigner           #+#    #+#             */
/*   Updated: 2024/02/11 03:46:19 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/history.h>

void	free_2d(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
}

void	free_all(t_common *c)
{
	t_env	*node;
	
	if (c)
	{
		while (c->env)
		{
			node = c->env;
			c->env = c->env->next;
			if (c->env->variable)
				free(c->env->variable);
			if (c->env->value)
				free(c->env->value);
			free(c->env);
		}
		if (c->tokenslist)
		{
			if (c->tokenslist->str)
				free_2d(c->tokenslist->str);
			free(c->tokenslist);
		}
		if (c->raw_prompt)
			free(c->raw_prompt);
		free(c);
	}
//	clear_history();
}

//3:40 still reachable: 215,467 bytes in 445 blocks

//3:45 definitely lost: 40 bytes in 1 blocks 
//still reachable: 214,516 bytes in 257 blocks