/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 13:21:04 by caigner           #+#    #+#             */
/*   Updated: 2024/03/12 19:03:48 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>

int	ft_single_quotes(char *str)
{
	if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
		return (1);
	else
	 	return (0);
}

char	*ft_create_string(char *env_value, char *str, int *i, int var_len)
{
	char	*tmp;
	char	*value;

	tmp = NULL;
	value = NULL;
	value = ft_substr(str, 0, *i);
	if (!value)
		return (ft_printerrno("expansion value malloc: "), NULL);
	tmp = ft_strjoin(value, env_value);
	if (!tmp)
		return (ft_printerrno("expansion tmp malloc: "), NULL);
	free(value);
	value = NULL;
	if (*i + var_len + 1 < (int)ft_strlen(str))
		value = ft_strjoin(tmp, &str[*i + var_len + 1]);
	else
		value = ft_strdup(tmp);
	if (!value)
		return (ft_printerrno("expansion value malloc: "), NULL);
	if (env_value)
		*i += var_len;
	free(tmp);
	tmp = NULL;
	return (value);
}

char	*ft_replace_var(t_env *env, char *str, int *i)
{
	int		var_len;
	char	*tmp;
	char	*value;

	tmp = NULL;
	value = NULL;
	var_len = 0;
	while (str[*i + 1 + var_len] && (ft_isalnum(str[*i + 1 + var_len]) || str[*i + 1 + var_len] == '_'))
		var_len++;
	tmp = ft_substr(str, *i + 1, var_len);
	if (!tmp)
		return (ft_printerrno("expansion: "), NULL);
	while (env)
	{
		if (!ft_strncmp(env->variable, tmp, var_len + 1))
			return (free(tmp), ft_create_string(env->value, str, i, var_len));
		env = env->next;
	}
	return (free(tmp), ft_create_string(NULL, str, i, var_len));
}

char	*ft_substitute(t_env *env, char *str)
{
	int		i;
	char	*ret;
	t_env	*tmp;

	ret = NULL;
	if (!str || !env)
		return (NULL);
	if (ft_single_quotes(str))
		return (str);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ')
		{
			tmp = env;
			ret = ft_replace_var(tmp, str, &i);
			if (!ret)
				return (ft_printerrno("expansion ret malloc: "), NULL);
//			free(str);
//			str = NULL;
			str = ret;
		}
		else
			i++;	
	}
	return (str);
}

void	ft_expand_red(t_env *env, t_io_red *io)
{
	if (!io)
		return ;
	io->infile = ft_substitute(env, io->infile);
	io->outfile = ft_substitute(env, io->outfile);
}

void	ft_expand_cmd(t_env *env, t_cmd_table *cmd)
{
	t_list	*tmp_io;
	t_list	*tmp_cmd;

	if (!cmd)
		return ;
	tmp_cmd = cmd->cmds;
	while (tmp_cmd)
	{
		tmp_cmd->content = ft_substitute(env, tmp_cmd->content);
		tmp_cmd = tmp_cmd->next;
	}
	tmp_io = cmd->io_red;
	while (tmp_io)
	{
		ft_expand_red(env, tmp_io->content);
		tmp_io = tmp_io->next;
	}
}

void	ft_expansion(t_env *env, t_list_d *cmds)//$? "|" ">" ...
{
	t_list_d	*tmp;

	tmp = cmds;
	while (tmp)
	{
		ft_expand_cmd(env, tmp->content);
		tmp = tmp->next;
	}
}

char	*ft_str_wo_quotes(char *str)
{
	char	*value;

	value = NULL;
	value = ft_strdup(str);
	return (value);
}

char	*ft_rm_quotes_str(char *str)
{
	int		i;
	char	*ret;

	ret = str;
	if (!str)
		return (NULL);
	i = ft_strlen(str);
	if ((str[0] == '\'' && str[i - 1] == '\'') || (str[0] == '"' && str[i - 1] == '"'))
	{
		str[i - 1] = 0;
		ret = ft_str_wo_quotes(&str[1]);
	}
	return (ret);
}

void	ft_rm_quotes_io(t_list *io_lst)
{
	t_io_red *io;

	while (io_lst)
	{
		io = io_lst->content;
		if (!io)
			return ;
		io->infile = ft_rm_quotes_str(io->infile);
		io->outfile = ft_rm_quotes_str(io->outfile);
		io_lst = io_lst->next;
	}
}

void	ft_rm_in_cmd(t_cmd_table *cmd)
{
	t_list	*tmp_cmd;
	t_list	*tmp_io;

	if (!cmd)
		return ;
	tmp_cmd = cmd->cmds;
	while (tmp_cmd)
	{
		tmp_cmd->content = ft_rm_quotes_str(tmp_cmd->content);
		tmp_cmd = tmp_cmd->next;
	}
	tmp_io = cmd->io_red;
	ft_rm_quotes_io(tmp_io);
}

void	ft_rm_quotes(t_list_d *cmds)
{
	t_list_d	*tmp;

	tmp = cmds;
	while (tmp)
	{
		ft_rm_in_cmd(tmp->content);
		tmp = tmp->next;
	}
}
