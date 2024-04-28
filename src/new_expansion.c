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

char	*get_expanded_str(t_common *c, char *str, int i)
{
	//show must go on here
}

int	varsize(char *str, int i)
{
	int		length;

	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		length++;
		i++;
	}
	return (length);
}

t_env *get_env_node(t_common *c, char *str, int i)
{
	int		length;
	t_env	*env;
	char	*var;

	length = varsize(str, i);
	var = malloc(length + 1);
	if (!var)
		return (printf("malloc-error"), NULL);
	ft_strlcpy(var, &str[i], length);
	env = c->env;
	while (env)
	{
		if (!strncmp(env->variable, var, ft_strlen(var))
				&& ft_strlen(env->variable) == length)
			return(free(var), env);
		env = env->next;
	}
	return (NULL);
}

char	*get_expansion_value(t_common *c, char *str, int i)
{
	int		lenght;
	t_env	*env_node;
	char	*env_value;

	if (str[i + 1] == '?')
		return (ft_itoa(c->exitstatus));
	else if (str[++i])
	{
		env_node = get_env_node(c, str, i);
		if (!env_node)
			return (NULL);
		return (env_node->value);
	}
	return (NULL);
}

char	*expand_str(t_common *c, char *str)
{
	int		i;
	char	*new;
	char	*envvalue;

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
			envvalue = get_expansion_value(c, str, i);
			new = get_expanded_str(c, str, i);
		}

	}
}

void	ft_expand_cmds(t_common *c, t_list *curr)
{
	char	*tmp;
	int		flag;

	flag = 0;
	while (curr)
	{
		while (has_expansion(c, curr->content))
		{
			tmp = ft_strdup(curr->content);
			free(curr->content);
			curr->content = expand_str(c, tmp);
			free(tmp);
		}
		curr = curr->next;
	}
}

void	ft_expand_io(t_common *c, t_list *curr)
{
	t_io_red	*io;
	char		*tmp;

	while(curr)
	{
		io = curr->content;
		if (io)
		{
			if (io->type == REDIR_IN)
			{
				while(has_expansion(c, io->infile))
				{
					tmp = ft_strdup(io->infile);
					free(io->infile);
					io->infile = expand_str(c, tmp);
					free(tmp);
				}
			}
			else if (io->type == REDIR_OUT || io->type == APPEND)
			{
				while(has_expansion(c, io->infile))
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
	t_cmd_table *cmd_struct;
	t_list      *curr;

	while (cmds)
	{
		cmd_struct = cmds->content;
		curr = cmd_struct->cmds;
		ft_expand_cmds(c, curr);
		if (cmd_struct->io_red)
		{
			curr = cmd_struct->io_red;
			ft_expand_io(c, curr);
		}
		cmds = cmds->next;
	}
}
