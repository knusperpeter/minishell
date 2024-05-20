/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 20:01:51 by caigner           #+#    #+#             */
/*   Updated: 2024/05/20 15:10:55 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	create_list_element(t_common *c, void **element, size_t size)
{
	*element = ft_protect(c, malloc(sizeof(char) * size), 0, 0);
	return (0);
}

int	ft_get_var_size(char *envp, char *eq)
{
	int	size;

	if (eq)
		size = eq - envp;
	else
		size = ft_strlen(envp);
	return (size);
}

int	ft_init_env(t_common *c, t_env *node, char *envp, t_env *prev)
{
	char	*eq;
	int		size;

	eq = ft_strchr(envp, '=');
	if (eq)
		if (*(eq - 1) == '+')
			eq -= 1;
	size = ft_get_var_size(envp, eq);
	node->variable = ft_protect(c, malloc(sizeof(char) * (size + 1)), 0, 0);
	ft_strlcpy(node->variable, envp, size + 1);
	node->flag = 0;
	if (eq)
	{
		if (*eq == '+')
			eq++;
		if (!ft_strncmp(node->variable, "SHLVL", 6))
			node->value = shlvl(c, eq + 1);
		else
			node->value = ft_protect(c, ft_strdup(eq + 1), node->variable, 0);
	}
	else
		node->value = NULL;
	node->prev = prev;
	return (node->next = NULL, EXIT_SUCCESS);
}

int	create_hidden_env(t_common *c, char **envp, int i, t_env **h_prev)
{
	t_env	*h_node;

	if (create_list_element(c, (void **) &h_node, sizeof(t_env)))
		return (free_env_nodes(c->hidden_env), 0);
	ft_init_env(c, h_node, envp[i], *h_prev);
	if (!c->hidden_env)
		c->hidden_env = h_node;
	else
		(*h_prev)->next = h_node;
	*h_prev = h_node;
	return (1);
}

int	dup_env(t_common *c, char **envp)
{
	int		i;
	t_env	*prev;
	t_env	*node;
	t_env	*h_prev;

	if (!envp)
		return (0);
	i = 0;
	prev = NULL;
	h_prev = NULL;
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
		if (!create_hidden_env(c, envp, i, &h_prev))
			return (0);
		i++;
	}
	return (1);
}
