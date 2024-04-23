#include "../include/minishell.h"

// flag für single quotes open and double quotes open. 
// if single quotes open ->skip until closed.
// if double quotes open expand while skipping until closed.

void	handle_conditions(t_common *common, char c)
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

void	expand_token(t_common *c, char *str)
{
	int	i;

	i = 0;
	c->open_double_quotes = 0;
	c->open_single_quotes = 0;
	while (str[i])
	{
		handle_conditions(c, str[i]);
		if (str[i] == '$' && !c->open_single_quotes && !ft_strchr(WHITESPACE, str[i + 1]))
			substitute_string(c, str, i);

			
	}
}

void	expansion(t_common *c, t_list_d *cmds)
{
	t_list_d	*curr;
	t_cmd_table	*cmd_struct;
	t_list		*curr_token;

	curr = cmds;
	while (curr)
	{
		cmd_struct = curr->content;
		curr_token = cmd_struct->cmds;
		while (curr_token)
		{
			expand_token(c, curr_token->content);
			curr_token = curr_token->next;
		}
		curr_token = cmd_struct->io_red;
		while (curr_token)
		{
			expand_token(c, curr_token->content);
			curr_token = curr_token->next;
		}
		curr = curr->next;
	}
}
