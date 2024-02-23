/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 02:56:18 by caigner           #+#    #+#             */
/*   Updated: 2024/02/20 16:26:52 by caigner          ###   ########.fr       */
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

void	free_env_nodes(t_env *start)
{
	t_env	*tmp;

	while (start)
	{
		tmp = start;
		start = start->next;
		if (tmp->variable)
			free(tmp->variable);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

void	free_cmd_table(void *content)
{
	int			i;
	t_cmd_table *table;
	
	table = (t_cmd_table *)content;
	if (table->str)
	{
		i = 0;
		while (table->str[i])
			free(table->str[i++]);
		free(table->str);
	}
}

void	free_all(t_common *c, t_cmd_table *cmds)
{
//	t_env	*node;
	//rl_clear_history();
	if (c)
	{
		free_env_nodes(c->env);
		if (c->cmd_struct)
		{
			if (cmds->str)
				free_2d(cmds->str);
			free(c->cmd_struct);
		}
		free(c);
	}
}
