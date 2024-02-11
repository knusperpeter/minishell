/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 20:01:51 by caigner           #+#    #+#             */
/*   Updated: 2024/02/11 03:38:56 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_env_nodes(t_env *start)
{
	t_env	*tmp;

	while (start)
	{
		tmp = start;
		start = start->next;
		if (start->variable)
			free(start->variable);
		if (start->value)
			free(start->value);
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
	int		i;
	int		size;

	i = 0;
	equals = ft_strchr(envp, '=');
	size = ft_get_var_size(envp, equals);
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
	if (equals)
		node->value = ft_strdup(equals + 1);//check if fails
	else
		node->value = NULL;
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
