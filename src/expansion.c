/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 19:49:04 by caigner           #+#    #+#             */
/*   Updated: 2024/05/12 01:30:03 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	has_expansion(t_common *c, char *str)
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
		if (str[i] == '$' && !c->open_single_quotes
			&& !ft_strchr(WHITESPACE, str[i + 1]))
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

char	*get_expanded_str(char *str, char *envvalue, int i, int varsize)
{
	char	*res;
	char	*tmp;
	char	*old_res;

	tmp = NULL;
	res = ft_substr(str, 0, i);
	//protect
	if (envvalue)
	{
		tmp = ft_strjoin(res, envvalue);
		free(res);
		res = NULL;
		//protect
	}
	old_res = res;
	if (tmp)
	{
		res = ft_strjoin(tmp, &str[i + varsize + 1]);
		free(tmp);
		tmp = NULL;
	//protect
	}
	if (old_res)
		free(old_res);
	free(envvalue);
	return (res);
}

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
	var = ft_substr(str, i, length);
	if (!var)
		return (printf("malloc-error"), NULL);
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

char	*get_expansion_value(t_common *c, char *str, int i, int *varsize)
{
	t_env	*env_node;

	if (str[i + 1] == '?')
		return (*varsize = 1, ft_itoa(c->exitstatus));
	else if (str[i + 1] == '$')
		return (*varsize = 1, ft_strdup("1589302"));
	else if (str[++i])
	{
		env_node = get_env_node(c, str, i);
		if (!env_node)
			return (*varsize = ft_varsize(str, i), ft_strdup(""));
		*varsize = ft_strlen(env_node->variable);
		return (ft_strdup(env_node->value));
	}
	return (NULL);
}

char	*expand_str(t_common *c, char *str)
{
	int		i;
	int		varsize;
	char	*new;
	char	*envvalue;

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
			envvalue = get_expansion_value(c, str, i, &varsize);
			new = get_expanded_str(str, envvalue, i, varsize);
			return (new);
		}
		i++;
	}
	return (NULL);
}

void	ft_split_cmd(t_list *curr)
{
	int		i;
	char	**tmp;
	t_list	*list;
	t_list	*new_list;
	t_list	*original_next;

	tmp = ft_split(curr->content, ' ');
	//protect
	i = 0;
	list = curr;
	original_next = curr->next;
	free(list->content);
	list->content = tmp[i];
	while (tmp[i])
	{
		if (tmp[i + 1])
		{
			new_list = ft_lstnew(tmp[i + 1]);
			new_list->next = list->next;
			list->next = new_list;
			list = list->next;
		}
		i++;
	}
	list->next = original_next;
	free(tmp);
}

int	split_command(t_list *lst)
{
	char	*str;
	int		last;

	if (!lst)
		return (0);
	str = lst->content;
	last = ft_strlen(str) - 1;
	if (last <= 0)
		return (0);
	if (ft_strchr("\'\"", str[0]) && ft_strchr("\'\"", str[last]))
		return (0);
	if (ft_strchr(lst->content, ' '))
		return (1);
	return (0);
}

int	check_for_empty(t_cmd_table **table, t_list **curr, t_list *before)
{
	char	*cmd;

	cmd = (*curr)->content;
	if (!cmd[0] && before)
	{
		before->next = (*curr)->next;
		free((*curr)->content);
		free(*curr);
		(*curr) = before->next;
		return (1);
	}
	else if (!cmd[0] && !before)
	{
		(*table)->cmds = (*curr)->next;
		free((*curr)->content);
		free(*curr);
		(*curr) = (*table)->cmds;
		return (1);
	}
	return (0);
}

void	ft_expand_cmds(t_common *c, t_cmd_table **table, t_list *curr)
{
	char	*tmp;
	t_list	*before;

	before = NULL;
	while (curr)
	{
		while (curr && has_expansion(c, curr->content))
		{
			tmp = curr->content;
			curr->content = expand_str(c, curr->content);
			free(tmp);
			if (!check_for_empty(table, &curr, before))
			{
				if (curr)
				{
					if (split_command(curr))
					{
						ft_split_cmd(curr);
					}
				}
			}
		}
		before = curr;
		if (curr)
			curr = curr->next;
	}
}

void	ft_expand_io(t_common *c, t_list *curr)
{
	t_io_red	*io;
	char		*tmp;

	while (curr)
	{
		io = curr->content;
		if (io)
		{
			if (io->type == REDIR_IN)
			{
				while (has_expansion(c, io->infile))
				{
					tmp = ft_strdup(io->infile);
					free(io->infile);
					io->infile = expand_str(c, tmp);
					free(tmp);
				}
			}
			else if (io->type == REDIR_OUT || io->type == APPEND)
			{
				while (has_expansion(c, io->outfile))
				{
					tmp = ft_strdup(io->outfile);
					free(io->outfile);
					io->outfile = expand_str(c, tmp);
					free(tmp);
				}
			}
		}
		curr = curr->next;
	}
}

void	ft_expansion(t_common *c, t_list_d *cmds)
{
	t_cmd_table	*cmd_struct;
	t_list		*curr;

	while (cmds)
	{
		cmd_struct = cmds->content;
		curr = cmd_struct->cmds;
		ft_expand_cmds(c, &cmd_struct, curr);
		if (cmd_struct->io_red)
		{
			curr = cmd_struct->io_red;
			ft_expand_io(c, curr);
		}
		cmds = cmds->next;
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
 * Description: Removes the quotes from the start and end of a string,if present
 * Parameters: str - The original string.
 * Returns: A new string with the quotes removed, or the original string if no
 * quotes were present.
 */
char	*ft_rm_quotes_str(char *str)
{
	int		i;
	int		j;
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
		if ((str[i] == '\'' && !double_quotes) || (str[i] == '\"'
				&& !single_quotes))
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
 * Description: Removes the quotes from the start and end of the filenames in
 * each I/O redirection in the list.
 * Parameters: io_lst - The linked list of I/O redirections.
 * This function iterates over the I/O redirection list and removes quotes from
 * the filenames.
 */
void	ft_rm_quotes_io(t_list *io_lst)
{
	t_io_red	*io;
	int			limiter_len;

	while (io_lst)
	{
		io = io_lst->content;
		if (!io)
			return ;
		io->infile = ft_rm_quotes_str(io->infile);
		io->outfile = ft_rm_quotes_str(io->outfile);
		if (io->heredoc_limiter)
		{
			limiter_len = ft_strlen(io->heredoc_limiter);
			io->heredoc_limiter = ft_rm_quotes_str(io->heredoc_limiter);
			if ((int)ft_strlen(io->heredoc_limiter) != limiter_len)
				io->expand_heredoc = 0;
		}
		io_lst = io_lst->next;
	}
}

/**
 * Function: ft_rm_in_cmd
 * Description: Removes the quotes from the start and end of the commands and
 * filenames in a command table.
 * Parameters: cmd - The command table.
 * This function iterates over the commands and I/O redirections in the command
 * table and removes quotes.
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
 * Description: Removes the quotes from the start and end of the commands and 
 * filenames in each command table in the list.
 * Parameters: cmds - The linked list of command tables.
 * This function iterates over the command tables and removes quotes in each 
 * one.
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
