/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:51:57 by caigner           #+#    #+#             */
/*   Updated: 2024/05/13 01:29:45 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_varsize(char *str, int i)
{
	int		length;

	length = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		length++;
		i++;
	}
	return (length);
}

static t_env	*get_env_node(t_common *c, char *str, int i)
{
	int		length;
	t_env	*env;
	char	*var;

	length = ft_varsize(str, i);
	var = ft_protect(c, ft_substr(str, i, length), 0, 0);
	env = c->env;
	while (env)
	{
		if (!ft_strncmp(env->variable, var, ft_strlen(var))
			&& (int)ft_strlen(env->variable) == length)
			return (free(var), env);
		env = env->next;
	}
	return (free(var), NULL);
}

char	*get_expansion_value(t_common *c, char *str, int i, int *len)
{
	t_env	*env_node;

	if (str[i + 1] == '?')
		return (*len = 1, ft_protect(c, ft_itoa(c->exitstatus), 0, 0));
	else if (str[i + 1] == '$')
		return (*len = 1, ft_protect(c, ft_strdup("1589302"), 0, 0));
	else if (str[++i])
	{
		env_node = get_env_node(c, str, i);
		if (!env_node)
		{
			*len = ft_varsize(str, i);
			return (ft_protect(c, ft_strdup(""), 0, 0));
		}
		*len = ft_strlen(env_node->variable);
		return (ft_protect(c, ft_strdup(env_node->value), 0, 0));
	}
	return (NULL);
}

char	*get_expanded_str(t_common *c, char *str, int i, int varsize)
{
	char	*envvalue;
	char	*res;
	char	*tmp;
	char	*old_res;

	tmp = NULL;
	res = ft_protect(c, ft_substr(str, 0, i), 0, 0);
	envvalue = get_expansion_value(c, str, i, &varsize);
	if (envvalue)
	{
		tmp = ft_protect(c, ft_strjoin(res, envvalue), res, 0);
		free(res);
		res = NULL;
	}
	old_res = res;
	if (tmp)
	{
		res = ft_protect(c, ft_strjoin(tmp, &str[i + varsize + 1]), tmp, 0);
		free(tmp);
		tmp = NULL;
	}
	if (old_res)
		free(old_res);
	return (free(envvalue), res);
}

char	*expand_str(t_common *c, char *str)
{
	int		i;
	int		varsize;
	char	*new;

	if (!str)
		return (NULL);
	i = 0;
	varsize = 0;
	c->open_double_quotes = 0;
	c->open_single_quotes = 0;
	while (str[i])
	{
		handle_quote_state(c, str[i]);
		if (str[i] == '$' && !c->open_single_quotes
			&& !ft_strchr(WHITESPACE, str[i + 1]))
		{
			new = get_expanded_str(c, str, i, varsize);
			return (new);
		}
		i++;
	}
	return (NULL);
}
