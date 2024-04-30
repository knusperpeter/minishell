/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 02:56:18 by caigner           #+#    #+#             */
/*   Updated: 2024/04/30 15:45:12 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * Function: free_2d
 * Description: Frees a 2D array of strings.
 * Parameter: str - The 2D array to free.
 */
void	free_2d(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
}
/**
 * Function: free_env_nodes
 * Description: Frees a linked list of environment variables.
 * Parameter: start - The start of the linked list to free.
 */
void	free_env_nodes(t_env *start)
{
	t_env	*tmp;

	while (start)
	{
		tmp = start;
		start = start->next;
		if (tmp->variable)
			free(tmp->variable);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}
/**
 * Function: free_tokens
 * Description: Frees a linked list of tokens.
 * Parameter: content - The linked list to free.
 */
void	free_tokens(void *content)
{
	t_token	*token;
	t_token	*tmp;
	
	tmp = (t_token *)content;
	while (tmp)
	{
		token = tmp;
		tmp = token->next;
		if (token->data)
			free(token->data);
		free (token);
	}
}
/**
 * Function: free_io_red
 * Description: Frees an IO redirection structure.
 * Parameter: content - The IO redirection structure to free.
 */
void	free_io_red(void *content)
{
	t_io_red	*io;
	
	io = (t_io_red *)content;
	if (io->heredoc_limiter)
		free(io->heredoc_limiter);
	if (io->infile)
		free(io->infile);
	if (io->outfile)
		free(io->outfile);
}
/**
 * Function: free_cmd_lst
 * Description: Frees a linked list of commands.
 * Parameter: lst - The linked list to free.
 */
void	free_cmd_lst(t_list **lst)
{
	t_list	*tmp;
	while (lst && *lst)
	{
		tmp = (*lst)->next;
		free(*lst);
		*lst = tmp;
	}
}
/**
 * Function: free_cmd_table
 * Description: Frees a command table structure.
 * Parameter: content - The command table structure to free.
 */
void	free_cmd_table(void *content)
{
	t_cmd_table *table;
	
	table = (t_cmd_table *)content;
	if (table->str)
		free_2d(table->str);
	if (table->exec_path)
		free(table->exec_path);
	if (table->io_red)
		ft_lstclear(&table->io_red, &free_io_red);
	if (table->cmds)
		free_cmd_lst(&table->cmds);
	if (table->heredoc_name)
	{
		unlink(table->heredoc_name);
		free(table->heredoc_name);
		table->heredoc_name = NULL;
	}
}

//Check here, since it fails

/**
 * Function: ft_cleanup_loop
 * Description: Cleans up after each iteration of the main loop.
 * Parameter: c - The common structure containing the shell data.
 */
void	ft_cleanup_loop(t_common *c)
{
	ft_lst_d_clear(&c->cmd_struct, &free_cmd_table);
	ft_lstclear(&c->tokens, &free_tokens);
	if (c->envp)
	{
		free_2d(c->envp);
		c->envp = NULL;
	}
	free(c->raw_prompt);
	c->raw_prompt = NULL;
}
/**
 * Function: free_all
 * Description: Frees all allocated memory before the program ends.
 * Parameter: c - The common structure containing the shell data.
 */
void	free_all(t_common *c, int cleanup_loop)
{
//	t_env	*node;
	//rl_clear_history();
	if (c)
	{
		if (cleanup_loop)
			ft_cleanup_loop(c);
		if (c->env)
			free_env_nodes(c->env);
		if (c->exitstatus_str)
			free(c->exitstatus_str);
//		free(c);
	}
	rl_clear_history();
}

void	ft_clean_exit(t_common *c, char *msg, int cleanup_loop)
{
	int	exitstatus;

	exitstatus = c->exitstatus;
	if (msg)
		printf("%s\n", msg);
//	close_all_pipes(c);
	free_all(c, cleanup_loop);
	exit(exitstatus);
}
