/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 20:01:51 by caigner           #+#    #+#             */
/*   Updated: 2024/05/13 01:29:45 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	create_list_element(t_common *c, void **element, size_t size)
{
	*element = ft_protect(c, malloc(sizeof(char) * size), 0, 0);
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

int	ft_init_env(t_common *c, t_env *node, char *envp, t_env *prev)
{
	char	*equals;
	int		size;

	equals = ft_strchr(envp, '=');
	if (equals)
		if (*(equals - 1) == '+')
			equals -= 1;
	size = ft_get_var_size(envp, equals);
	node->variable = ft_protect(c, malloc(sizeof(char) * (size + 1)), 0, 0);
	ft_strlcpy(node->variable, envp, size + 1);
	node->flag = 0;
	if (equals)
	{
		if (*equals == '+')
			equals++;
		node->value = ft_protect(c, ft_strdup(equals + 1), node->variable, 0);
	}
	else
		node->value = NULL;
	node->prev = prev;
	return (node->next = NULL, EXIT_SUCCESS);
}

int	dup_env(t_common *c, char **envp)
{
	int		i;
	t_env	*prev;
	t_env	*node;

	if (!envp)
		return (0);
	i = 0;
	prev = NULL;
	while (envp[i])
	{
		if (create_list_element(c, (void **) &node, sizeof(t_env)))
			return (free_env_nodes(c->env), 0);
		ft_init_env(c, node, envp[i], prev);
		if (!c->env)
			c->env = node;
		else
			prev->next = node;
		prev = node;
		i++;
	}
	return (1);
}
