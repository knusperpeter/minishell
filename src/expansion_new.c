#include "../include/minishell.h"
#include <limits.h>

// flag für single quotes open and double quotes open. 
// if single quotes open ->skip until closed.
// if double quotes open expand while skipping until closed.

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

char	*replace_str(char **str, int i, int varlen, char *value)
{
	int		valuelen;
	char	*tmp;

	valuelen = ft_strlen(value);
	tmp = malloc(sizeof(char) * (ft_strlen(*(str) - varlen + valuelen + 1)));
	if (!tmp)
		return (dprintf(2 ,"mallocfail"), NULL);
	
}

char	*get_env_value(t_env *env, char *env_var)
{
	while (env)
	{
		if (!ft_strncmp(env->variable, env_var, ft_strlen(env_var)))
			return (ft_strdup(env->variable));
		env = env->next;
	}
	return (NULL);
}

void	replace_with_value(t_common *c, char **str, int i)
{
	int		varlen;
	int		tmp;
	char	*env_var;
	char	*env_value;

	varlen = 1;
	tmp = i;
	if (*(str[i + 1]) == '?')
	{
		c->exitstatus_str = ft_itoa(c->exitstatus);
		*str = replace_str(str, i, 2, c->exitstatus_str);
		return ;
	}
	else if (*(str[i + 1]))
	{
		tmp++;
		while (ft_isalnum(*(str[tmp])) || *(str[tmp]) == '_')
			varlen++;
		env_var = malloc(sizeof(char) * varlen);
		if (!env_var)
		{
			dprintf(2, "mallocfail");
			return ;
		}
	//problem hier weiter	if (ft_strlcpy(env_var, &*(str[i]), varlen) != varlen)
			return ;
		env_value = get_env_value(c->env, env_var);
		*str = replace_str(str, i, varlen, env_value);
	}
}

void	expand_token(t_common *c, char *str)
{
	int	i;

	i = 0;
	c->open_double_quotes = 0;
	c->open_single_quotes = 0;
	while (str[i])
	{
		handle_quote_state(c, str[i]);
		if (str[i] == '$' && !c->open_single_quotes && !ft_strchr(WHITESPACE, str[i + 1]))
			replace_with_value(c, &str, i);
	}
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

void	expansion(t_common *c, t_list_d *cmds)
{
	t_list_d	*curr;
	t_cmd_table	*cmd_struct;
	t_list		*curr_token;
	t_io_red	*io_struct;

	curr = cmds;
	while (curr)
	{
		cmd_struct = curr->content;
		curr_token = cmd_struct->cmds;
		while (curr_token)
		{
			expand_token(c, curr_token->content);
			split_whitespace(c, curr_token->content);
			curr_token = curr_token->next;
		}
		curr_token = cmd_struct->io_red;
		io_struct = curr_token->content;
		while (curr_token)
		{
			expand_io_token(c, io_struct);
			curr_token = curr_token->next;
		}
		curr = curr->next;
	}
}
