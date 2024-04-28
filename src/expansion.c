/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:09:34 by miheider          #+#    #+#             */
/*   Updated: 2024/04/28 21:13:57 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// DER SCHEI? ist im quotes removal

#include "../include/minishell.h"
#include <readline/chardefs.h>

void	handle_quote_state(t_common *common, char c)
{
	if (c == '\'' && !common->open_double_quotes)
	{
		if (common->open_single_quotes)
			common->open_single_quotes = 0;
		else
			common->open_single_quotes = 1;
	}
	else if (c == '\"' && !common->open_single_quotes)
	{
		if (common->open_double_quotes)
			common->open_double_quotes = 0;
		else
			common->open_double_quotes = 1;
	}
}

char *replace_str(char *str, int i, int varlen, char *value)
{
    if (!str || i < 0 || i + varlen > (int)strlen(str) || !value) {
        return NULL;
    }
    char	*tmp1 = ft_substr(str, 0, i);
    if (!tmp1) {
        return NULL;
    }
    char	*tmp2 = ft_strjoin(tmp1, value);
    free(tmp1);
    if (!tmp2) {
        return NULL;
    }
    tmp1 = ft_strjoin(tmp2, str + i + varlen);
    free(tmp2);
    if (!tmp1) {
        return NULL;
    }
    return tmp1;
}

static char	*get_env_value(t_env *env, char *env_var)
{
	while (env)
	{
		if (!ft_strncmp(env->variable, env_var, ft_strlen(env_var)))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

char	*replace_with_env(t_common *c, int varlen, int i, char *str)
{
	char	*env_var;
	char	*env_value;
	char	*tmp;
	
	env_var = malloc(sizeof(char) * (varlen + 1));
	if (!env_var)
	{
		dprintf(2, "mallocfail");
		return (str);
	}
	if (ft_strlcpy(env_var, str + i + 1, varlen - 1) != (size_t)varlen - 1)
		return (str);
	env_value = get_env_value(c->env, env_var);
	if (!env_value)
		return (printf("malloc fail"), NULL);
	tmp = replace_str(str, i, varlen, env_value);
	return (tmp);
}

char	*replace_with_value(t_common *c, char *str, int i)
{
	int		varlen;
	int		tmp;

	varlen = 1;
	tmp = i;
	if (str[i + 1] == '?')
	{
		c->exitstatus_str = ft_itoa(c->exitstatus);
		return (replace_str(str, i, 2, c->exitstatus_str));
	}
	else if (str[i + 1])
	{
		tmp++;
		while (str[tmp] && (ft_isalnum(str[tmp]) || str[tmp] == '_'))
		{
			varlen++;
			tmp++;
		}
		return (replace_with_env(c, varlen, i, str));
	}
}


char	*expand_token(t_common *c, char *str)
{
	int		i;
	char	*new_str;

	if (!str)
		return (NULL);
	i = 0;
	c->open_double_quotes = 0;
	c->open_single_quotes = 0;
	while (str[i])
	{
		handle_quote_state(c, str[i]);
		if (str[i] == '$' && !c->open_single_quotes && !ft_strchr(WHITESPACE, str[i + 1]))
		{
			new_str = replace_with_value(c, str, i);
			return (new_str);
		}
		else
			i++;
	}
	return (str);
}

void	expand_io_token(t_common *c, t_io_red *io_red)
{
	if (io_red)
	{
		if (io_red->type == REDIR_IN)
			expand_token(c, io_red->infile);
		else if (io_red->type == REDIR_OUT || io_red->type == APPEND)
			expand_token(c, io_red->outfile);
	}
}

int	dollar_sign_expansion(t_common *c, char *str)
{
	char	*curr_dollar;
	int		i;
	
	i = 0;
	c->open_double_quotes = 0;
	c->open_single_quotes = 0;
	curr_dollar = ft_strchr(str, '$');
	while (str[i] && curr_dollar)
	{
		handle_quote_state(c, str[i]);
		if (str[i] == '$' && !c->open_single_quotes && !ft_strchr(WHITESPACE, str[i + 1]))
		{
			if (curr_dollar == &str[i])
				return (1);
			else
				curr_dollar = ft_strchr(curr_dollar + 1, '$');
		}
		i++;
	}
	return (0);
}

void	ft_expansion(t_common *c, t_list_d *cmds)
{
	t_list_d	*subcommand;
	t_cmd_table	*cmd_struct;
	t_list		*curr_token;
	t_io_red	*io_struct;
	char		*old_string;

	subcommand = cmds;
	while (subcommand)
	{
		cmd_struct = subcommand->content;
		curr_token = cmd_struct->cmds;
		while (curr_token)
		{
			printf("Token: %s\n", (char *)curr_token->content);
			while (dollar_sign_expansion(c, curr_token->content))
			{
				old_string = ft_strdup(curr_token->content);
				printf("string : %s\n", old_string);
				curr_token->content = expand_token(c, old_string);
				printf("after strdup: %s\n", (char *)curr_token->content);
				free(old_string);
			}
			printf("Token after expansion: %s\n", (char*)curr_token->content);
		//	split_whitespace(c, curr_token->content);
			curr_token = curr_token->next;
		}
		if (cmd_struct->io_red)
		{
			curr_token = cmd_struct->io_red;
			io_struct = curr_token->content;
			while (curr_token)
			{
				expand_io_token(c, io_struct);
				curr_token = curr_token->next;
			}
		}
		subcommand = subcommand->next;
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
 * Function: ft_rm_quotes_strtmp2
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
