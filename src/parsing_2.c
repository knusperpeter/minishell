#include "../include/minishell.h"

int	tokenize(t_common *c)
{
	t_list	*cmd_tok;
	t_token	*token;
	char	**arr;
	char	**sub_arr;
	int		i;

	arr = tokenize_one(c, c->raw_prompt, count_pipes(c, c->raw_prompt) + 1);
	if (!arr)
		return (EXIT_FAILURE);
	i = 0;
	while (arr[i])
	{
		sub_arr = prep_input(arr[i++]);
		token = malloc(sizeof(t_token));
		if (!token)
			return (c->exitstatus = 1, ft_clean_exit(c, "malloc-fail", 0), 1);
		cmd_tok = ft_lstnew(token);
		if (!cmd_tok)
			return (c->exitstatus = 1, free(token),
				ft_clean_exit(c, "malloc-fail", 0), 1);
		add_to_list(sub_arr, cmd_tok);
		ft_lstadd_back(&c->tokens, cmd_tok);
		free_2d(sub_arr);
	}
	return (free_2d(arr), EXIT_SUCCESS);
}

int	cmd_to_node(t_cmd_table *cmd_node)
{
	int			i;
	t_list		*cmd_tok;
	t_cmd_table	*cmd_tmp;

	cmd_tmp = cmd_node;
	cmd_tok = cmd_tmp->cmds;
	i = ft_lstsize(cmd_tok);
	cmd_tmp->str = malloc(sizeof(char **) * (i + 1));
	if (!cmd_tmp->str)
		return (perror("Error initializing str in cmd_to_node\n"), 1);
	cmd_tok = cmd_tmp->cmds;
	i = 0;
	while (cmd_tok)
	{
		cmd_tmp->str[i] = ft_strdup(cmd_tok->content);
		if (!cmd_tmp->str[i])
			printf("FIX, cmd_to_node\n");
		i++;
		cmd_tok = cmd_tok->next;
	}
	cmd_tmp->str[i] = NULL;
	return (EXIT_SUCCESS);
}

void	ft_cmd_args_to_2d(t_list_d *cmd_table)
{
	t_list_d	*tmp;

	tmp = cmd_table;
	while (tmp)
	{
		cmd_to_node(tmp->content);
		tmp = tmp->next;
	}
}
