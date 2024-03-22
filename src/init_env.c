/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 20:01:51 by caigner           #+#    #+#             */
/*   Updated: 2024/03/22 01:50:35 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	create_list_element(void **element, size_t size)
{
	*element = malloc(sizeof(char) * size);
	if (!(*element))
		return (1);
	return (0);
}

int	ft_get_var_size(char *envp, char *equals)
{
	int	size;

	if (equals)
		size = equals - envp;
	else
		size = ft_strlen(envp);
	return (size);
}

int	ft_init_env(t_env *node, char *envp, t_env *prev)
{
	char	*equals;
	int		size;

	equals = ft_strchr(envp, '=');
	size = ft_get_var_size(envp, equals);
	node->variable = malloc(sizeof(char) * (size + 1));
	if (!node->variable)
		return (EXIT_FAILURE);
	ft_strlcpy(node->variable, envp, size +1);
	node->flag = 0;
	if (equals)
	{
		node->value = ft_strdup(equals + 1);
		if (!node->value)
			return (EXIT_FAILURE);
	}
	else
		node->value = NULL;
	node->prev = prev;
	return (node->next = NULL, EXIT_SUCCESS);
}

int	dup_env(t_common *c, char **envp)
{
	int		i;
	t_env	*start;
	t_env	*prev;
	t_env	*node;

	if (!envp)
		return (0);
	i = 0;
	start = NULL;
	prev = NULL;
	while (envp[i])
	{
		if (create_list_element((void **) &node, sizeof(t_env)))
			return (free_env_nodes(start), 0);
		ft_init_env(node, envp[i], prev);
		if (!start)
			start = node;
		else
			prev->next = node;
		prev = node;
		i++;
	}
	c->env = start;
	return (1);
}
