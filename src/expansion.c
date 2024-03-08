/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 13:21:04 by caigner           #+#    #+#             */
/*   Updated: 2024/03/08 21:55:35 by caigner          ###   ########.fr       */
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
	ft_strlcpy(value, str, *i - 1);
	free(str);
	tmp = ft_strjoin(value, env_value);
	free(value);
	value = ft_strjoin(tmp, &str[*i + var_len + 1]);
	*i += ft_strlen(env_value);
	free(str);
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
	var_len = 1;
	while (str[*i + var_len] && str[*i + var_len] != ' ')
		var_len++;
	ft_strlcpy(tmp, &str[*i + 1], var_len);
	if (!tmp)
		return (/* ft_printerrno("expansion: "),  */NULL);
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
	while (str[i] && str[i + 1])
	{
		if (str[i] == '$' && str[i + 1] != ' ')
		{
			tmp = env;
			ret = ft_replace_var(tmp, str, &i);
			free(str);
			str = ret;
		}
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
	int		i;
	t_list	*tmp;

	if (!cmd)
		return ;
	tmp = cmd->io_red;
	i = 0;
	while (cmd->str[i])
	{
		cmd->str[i] = ft_substitute(env, cmd->str[i]);
		i++;
	}
	while (tmp)
	{
		ft_expand_red(env, tmp->content);
		tmp = tmp->next;
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

char	*ft_str_wo_quotes(char *str, int size)
{
	char	*value;

	value = NULL;
	ft_strlcpy(value, str, size);
	return (value);
}

char	*ft_rm_quotes_str(char *str)
{
	int		i;
	char	*ret;

	ret = NULL;
	if (!str)
		return (NULL);
	i = ft_strlen(str) - 1;
	if ((str[0] == '\'' && str[i] != '\'') || (str[0] == '"' && str[i] != '"'))
	{
		ret = ft_str_wo_quotes(&str[1], i - 1);
		free(str);
	}
	return (ret);
}

void	ft_rm_quotes_io(t_io_red *io)
{
	if (!io)
		return ;
	io->infile = ft_rm_quotes_str( io->infile);
	io->outfile = ft_rm_quotes_str( io->outfile);
}

void	ft_rm_in_cmd(t_cmd_table *cmd)
{
	int		i;
	t_list	*tmp;

	if (!cmd)
		return ;
	tmp = cmd->io_red;
	i = 0;
	while (cmd->str[i])
	{
		cmd->str[i] = ft_rm_quotes_str(cmd->str[i]);
		i++;
	}
	while (tmp)
	{
		ft_rm_quotes_io(tmp->content);
		tmp = tmp->next;
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
