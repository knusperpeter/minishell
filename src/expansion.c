/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:09:34 by miheider          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/03/25 17:07:27 by caigner          ###   ########.fr       */
=======
/*   Updated: 2024/03/30 13:33:34 by chris            ###   ########.fr       */
>>>>>>> 8f095c83947716ed3fbe3036b81b0d35cbce1560
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

<<<<<<< HEAD
int	dont_expand_result(char *str, int i, int double_quotes, int single_quotes)
{
	int	j;

	j = i;
	while (i != -1)
	{
		if (str[i] == '\'' && nb_esc_char(str, i) % 2 == 0)
			single_quotes++;
		if (str[i] == '\"' && nb_esc_char(str, i) % 2 == 0)
			double_quotes++;
		i--;
	}
	if (single_quotes == 1 && double_quotes == 0)
		return (1);
	else if (single_quotes == 1 && str[j - 1] == '\"')
		return (1);
	else if (single_quotes == 2 && double_quotes == 1)
		return (0);
	else if (single_quotes == 1 && double_quotes == 2 && str[j - 1] == '\'')
		return (1);
	else if (single_quotes == 2 && double_quotes == 2 && str[j - 1] == '\''
			&& str[j - 3] == '\'')
		return (1);
	else if (single_quotes == 4 && double_quotes == 3)
		return (1);
	return (0);
}

int	dont_expand(char *str, int i)
{
	int	j;
	int	single_quotes;
	int	double_quotes;
	int	go_back;

	single_quotes = 0;
	double_quotes = 0;
	go_back = 0;
	if (i == 0)
		return (0);
	j = i;
	if (i > 0 && (str[i + 1] == '\"' || str[i + 1] == '\''))
		return (1);
	while (j != 0)
	{
		if (str[j] == '\"' || str[j] == '\"')
			go_back = 0;
		j--;
	}
	if (go_back == 0)
		if (dont_expand_result(str, i, double_quotes, single_quotes))
			return (1);
=======
int dont_expand_result(char *str, int i)
{
	int inside_single_quotes = 0;
	int inside_double_quotes = 0;

	while (i >= 0)
	{
		if (str[i] == '\'' && nb_esc_char(str, i) % 2 == 0 && !inside_double_quotes)
		{
			inside_single_quotes = !inside_single_quotes;
		}
		else if (str[i] == '\"' && nb_esc_char(str, i) % 2 == 0)
		{
			inside_double_quotes = !inside_double_quotes;
		}
		i--;
	}
	return (inside_single_quotes);
}

int	dont_expand(char *str, int i)
{
	if (i == 0)
		return (0);
	if (i > 0 && (str[i + 1] == '\"' || str[i + 1] == '\''))
		return (1);
	if (dont_expand_result(str, i))
		return (1);
>>>>>>> 8f095c83947716ed3fbe3036b81b0d35cbce1560
	return (0);
}

int	nb_esc_char(char *str, int index)
{
	int	i;
	int	count;

	i = index - 1;
	count = 0;
	if (index <= 0)
		return (count);
<<<<<<< HEAD
	while (str[i] != -1)
=======
	while (i != -1)
>>>>>>> 8f095c83947716ed3fbe3036b81b0d35cbce1560
	{
		if (str[i] == '\\')
			count++;
		else if (str[i] != '\\')
			return (count);
		i--;
	}
	return (count);
}

int	has_dollar(t_common *c, char *str)
{
	int	i;

	(void)c;
	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[0] == '$' && !ft_strchr(WHITESPACE, str[i + 1]) && str[i + 1])
			return (1);
		else if (str[i] == '$' && !str[i + 1])
			break;
		else if ((str[i] == '$' && nb_esc_char(str, i) % 2 != 0) || dont_expand(str, i))
			i++;
		else if (str[i] == '$' && ft_strchr(WHITESPACE, str[i + 1]))
			return (0);
		else if (str[i] == '$' && !ft_strchr(WHITESPACE, str[i + 1]))
			return (1);
		i++;
	}
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

	if (env_value == NULL)
		return (ft_strdup(str));
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
	{
		c->exitstatus_str = ft_itoa(c->exitstatus);
		if (!c->exitstatus_str)
			return (ft_printerrno("expansion: "), NULL);
		return (ft_create_string(c->exitstatus_str, str, i, 1));
	}
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

	ret = NULL;
	if (!str || !env)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && nb_esc_char(str, i) % 2 == 0
			&& !dont_expand(str, i))
		{
			tmp = env;
			ret = ft_replace_var(c, tmp, str, &i);
			if (!ret)
				return (ft_printerrno("expansion ret malloc: "), NULL);
			str = ret;
			if (ft_strchr(str, ' '))
			{
				
			}
		}
		i++;
	}
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
		if (has_dollar(c, io->infile))
			io->infile = ft_substitute(c, c->env, io->infile);
		if (has_dollar(c, io->outfile))
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
void	ft_expand_cmd_table(t_common *c, t_cmd_table *cmd)
{
	t_list	*tmp_lst;
	t_list	*tmp_cmd;
	char	**tmp1;
	int		i;

	if (!cmd)
		return ;
	tmp_cmd = cmd->cmds;
	while (tmp_cmd)
	{
		if (has_dollar(c, tmp_cmd->content))
<<<<<<< HEAD
			tmp_cmd->content = ft_substitute(c, c->env, tmp_cmd->content);
		tmp_cmd = tmp_cmd->next;
	}	
	tmp_io = cmd->io_red;
	ft_expand_red(c, tmp_io);
=======
		{
			tmp_cmd->content = ft_substitute(c, c->env, tmp_cmd->content);
			if (ft_strchr(tmp_cmd->content, ' '))
			{
				tmp1 = ft_split(tmp_cmd->content, ' ');
				if (!tmp1)
					return ;
				free(tmp_cmd->content);
				tmp_cmd->content = ft_strdup(tmp1[0]);
				i = 1;
				while (tmp1[i])
				{
					tmp_lst = ft_lstnew(ft_strdup(tmp1[i]));
					tmp_lst->next = tmp_cmd->next;
					tmp_cmd->next = tmp_lst;
					tmp_cmd = tmp_cmd->next;
					i++;
				}
				free (tmp1);
			}
		}
		tmp_cmd = tmp_cmd->next;
	}
	
	tmp_lst = cmd->io_red;
	ft_expand_red(c, tmp_lst);
>>>>>>> 8f095c83947716ed3fbe3036b81b0d35cbce1560
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
	t_list		*cmd_table;
	t_token		*cmd;	

	tmp = cmds;
	while (tmp)
	{
<<<<<<< HEAD
		cmd_table = tmp->content;
		cmd = cmd_table->content;
=======
>>>>>>> 8f095c83947716ed3fbe3036b81b0d35cbce1560
		ft_expand_cmd_table(c, tmp->content);
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
