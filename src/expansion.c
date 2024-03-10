/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@42>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 13:21:04 by caigner           #+#    #+#             */
/*   Updated: 2024/03/10 20:06:40 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	tmp = ft_strjoin(value, env_value);
	free(value);
	if (*i + var_len + 1 < (int)ft_strlen(str))
		value = ft_strjoin(tmp, &str[*i + var_len + 1]);
	else
		value = ft_strdup(tmp);
	if (env_value)
		*i += var_len;
	free(tmp);
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
	if (!str)
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
			free(str);
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
	printf("%s\n", str);
	i = ft_strlen(str);
	if ((str[0] == '\'' && str[i - 1] == '\'') || (str[0] == '"' && str[i - 1] == '"'))
	{
		str[i - 1] = 0;
		ret = ft_str_wo_quotes(&str[1]);
		//free(str);
	}
	printf("%s\n", ret);
	return (ret);
}

void	ft_rm_quotes_io(t_io_red *io)
{
	if (!io)
		return ;
	io->infile = ft_rm_quotes_str(io->infile);
	io->outfile = ft_rm_quotes_str(io->outfile);
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
	while (tmp_io)
	{
		ft_rm_quotes_io(tmp_io->content);
		tmp_io = tmp_io->next;
	}
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
