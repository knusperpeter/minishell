/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 20:17:44 by chris             #+#    #+#             */
/*   Updated: 2024/02/03 18:48:55 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#include <stdlib.h>

void	free_env_nodes(t_env *start)
{
	t_env	*tmp;

	while (start)
	{
		tmp = start;
		start = start->next;
		free(tmp->variable);
		free(tmp);
	}
}

int	create_list_element(void **element, size_t size)
{
	*element = malloc(size);
	if (*element == NULL)
		return (1);
	return (0);
}

int	init_env(t_env *node, char *envp, t_env *prev)
{
	char	*equals;
	int		i;
	int		size;
	
	i = 0;
	equals = ft_strchr(envp, '=');
	size = equals - envp;
	node->variable = malloc(size + 1);
	if (!node->variable)
		return (EXIT_FAILURE);
	while (i < size)
	{
		node->variable[i] = envp[i];
		i++;
	}
	node->variable[i] = 0;
	node->flag = 0;
	if (!*(equals + 1))
		return (node->flag = 1, EXIT_SUCCESS);
	node->value = ft_strdup(equals + 1);
	node->prev = prev;
	node->next = NULL;
	return (EXIT_SUCCESS);
}

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
		if (create_list_element((void **) &node, sizeof(t_env)))
		{
			free_env_nodes(start);
			return (1);
		}
		init_env(node, envp[i], prev);
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
	(void)		ac;
	(void)		av;

/* 	while (1)
	{
		printf("%s@%s:%s", c.prompt->username, c.prompt->hostname, c.prompt->prompt_text);
	} */
	if (envp)
		dup_env(&c, envp);
	ft_export(NULL, c.env);
	return (0);
}
