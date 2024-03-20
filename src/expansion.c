/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:09:34 by miheider          #+#    #+#             */
/*   Updated: 2024/03/20 13:43:24 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>

//	wenn quotes in doublequotes, dann quotes nicht removen
//	wenn doublequotes in quotes, dann nichts expanden
int	ft_single_quotes(char *str)
{
	if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
		return (1);
	else
	 	return (0);
}

/**
 * Function: ft_create_string
 * Description: Creates a new string by replacing a variable in the original string with its value from the environment.
 * Parameters: env_value - The value of the variable in the environment, str - The original string, i - The index of the variable in the string, var_len - The length of the variable.
 * Returns: The new string with the variable replaced by its value.
 */
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

/**
 * Function: ft_replace_var
 * Description: Replaces a variable in a string with its value from the environment.
 * Parameters: env - The linked list of environment variables, str - The original string, i - The index of the variable in the string.
 * Returns: The new string with the variable replaced by its value.
 */
char	*ft_replace_var(t_common *c, t_env *env, char *str, int *i)
{
	int		var_len;
	char	*tmp;

	tmp = NULL;
	var_len = 0;
	while (str[*i + 1 + var_len] && (ft_isalnum(str[*i + 1 + var_len]) || str[*i + 1 + var_len] == '_'))
		var_len++;
	if (str[*i + 1] == '?')
		tmp = ft_itoa(c->exitstatus);
	else
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

/**
 * Function: ft_substitute
 * Description: Substitutes variables in a string with their corresponding values from the environment.
 * Parameters: env - The linked list of environment variables, str - The original string.
 * Returns: The new string with all variables replaced by their values.
 */
char	*ft_substitute(t_common *c, t_env *env, char *str)
{
	int		i;
	char	*ret;
	t_env	*tmp;
	int		single_quotes;
	int		double_quotes;

	ret = NULL;
	single_quotes = 0;
	double_quotes = 0;
	if (!str || !env)
		return (NULL);
	i = 0;
	while (str[i])
	{//das gehört in den lexer
		if (str[i] == '\'' && !single_quotes && !double_quotes)
			single_quotes = 1;
		else if (str[i] == '\'' && single_quotes && !double_quotes)
			single_quotes = 0;
		else if (str[i] == '\"' && !single_quotes && double_quotes)
			double_quotes = 0;
		else if (str[i] == '\"' && !single_quotes && !double_quotes)
			double_quotes = 1;
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ' && !single_quotes)
		{
			tmp = env;
			ret = ft_replace_var(c, tmp, str, &i);
			if (!ret)
				return (ft_printerrno("expansion ret malloc: "), NULL);
			str = ret;
		}
		else
			i++;
	}
	if (single_quotes || double_quotes)
		return (printf("Error: unclosed quotes: %s\n", str), NULL); //should we print the right arg?
	return (str);
}

/**
 * Function: ft_expand_red
 * Description: Expands variables in the input and output filenames of each I/O redirection in the list.
 * Parameters: env - The linked list of environment variables, io_lst - The linked list of I/O redirections.
 * This function iterates over the I/O redirection list and substitutes variables in the filenames.
 */
void	ft_expand_red(t_common *c, t_list *io_lst)
{
	t_io_red	*io;

	while (io_lst)
	{
		io = io_lst->content;
		if (!io)
			return ;
		io->infile = ft_substitute(c, c->env, io->infile);
		io->outfile = ft_substitute(c, c->env, io->outfile);
		io_lst = io_lst->next;
	}
}

/**
 * Function: ft_expand_cmd
 * Description: Expands variables in the command and I/O redirections of a command table.
 * Parameters: env - The linked list of environment variables, cmd - The command table.
 * This function iterates over the command and I/O redirections and substitutes variables.
 */
void	ft_expand_cmd(t_common *c, t_cmd_table *cmd)
{
	t_list	*tmp_io;
	t_list	*tmp_cmd;

	if (!cmd)
		return ;
	tmp_cmd = cmd->cmds;
	while (tmp_cmd)
	{
		tmp_cmd->content = ft_substitute(c, c->env, tmp_cmd->content);
		tmp_cmd = tmp_cmd->next;
	}
	tmp_io = cmd->io_red;
	ft_expand_red(c, tmp_io);
}

/**
 * Function: ft_expansion
 * Description: Expands variables in the commands and I/O redirections of each command table in the list.
 * Parameters: env - The linked list of environment variables, cmds - The linked list of command tables.
 * This function iterates over the command tables and expands variables in each one.
 */
void	ft_expansion(t_common *c, t_list_d *cmds)//$? "|" ">" ...
{
	t_list_d	*tmp;

	tmp = cmds;
	while (tmp)
	{
		ft_expand_cmd(c, tmp->content);
		tmp = tmp->next;
	}
}

/**
 * Function: ft_str_wo_quotes
 * Description: Creates a duplicate of the input string.
 * Parameters: str - The original string.
 * Returns: A new string which is a duplicate of the input string.
 */
char	*ft_str_wo_quotes(char *str)
{
	char	*value;

	value = NULL;
	value = ft_strdup(str);
	return (value);
}

/**
 * Function: ft_rm_quotes_str
 * Description: Removes the quotes from the start and end of a string, if present.
 * Parameters: str - The original string.
 * Returns: A new string with the quotes removed, or the original string if no quotes were present.
 */
char	*ft_rm_quotes_str(char *str)
{
	int		i;
	int 	j;
	int		single_quotes;
	int		double_quotes;

	if (!str)
		return (NULL);
	single_quotes = 0;
	double_quotes = 0;
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '\'' && !double_quotes) || (str[i] == '\"' && !single_quotes))
		{
			if (str[i] == '\'' && !single_quotes)
				single_quotes = 1;
			else if (str[i] == '\'' && single_quotes)
				single_quotes = 0;
			else if (str[i] == '"' && !double_quotes)
				double_quotes = 1;
			else if (str[i] == '"' && double_quotes)
				double_quotes = 0;
		}
		else
			str[j++] = str[i];
		i++;
	}
	str[j] = 0;
//	printf("%s\n", str);
	return (str);
}

/**
 * Function: ft_rm_quotes_io
 * Description: Removes the quotes from the start and end of the filenames in each I/O redirection in the list.
 * Parameters: io_lst - The linked list of I/O redirections.
 * This function iterates over the I/O redirection list and removes quotes from the filenames.
 */
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

/**
 * Function: ft_rm_in_cmd
 * Description: Removes the quotes from the start and end of the commands and filenames in a command table.
 * Parameters: cmd - The command table.
 * This function iterates over the commands and I/O redirections in the command table and removes quotes.
 */
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

/**
 * Function: ft_rm_quotes
 * Description: Removes the quotes from the start and end of the commands and filenames in each command table in the list.
 * Parameters: cmds - The linked list of command tables.
 * This function iterates over the command tables and removes quotes in each one.
 */
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
