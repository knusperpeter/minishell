/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 20:17:44 by chris             #+#    #+#             */
/*   Updated: 2024/01/31 18:32:22 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	dup_env(t_common *c, char **envp)
{
	int		i;
	t_env	*start;
	t_env	*prev;
	t_env	*node;

	i = 0;
	start = NULL;
	prev = NULL;
	while (envp[i])
	{
		node = malloc(sizeof(t_env));
		if (!node)
		{
			free_nodes(start);
			return (-1);
		}
		node->variable = ft_strdup(envp[i]);
		node->flag = 0;
		if (!*(ft_strchr(node->variable, '=') + 1))
			node->flag = 1;
		node->prev = prev;
		node->next = NULL;
		if (!start)
			start = node;
		else
			prev->next = node;
		prev = node;
		i++;
	}
	c->env = start;
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_common	c;

	while (1)
	{
		printf("%s@%s:%s", c.prompt->username, c.prompt->hostname, c.prompt->prompt_text);
	}
	if (envp)
		dup_env(&c, envp);
	return (0);
}
