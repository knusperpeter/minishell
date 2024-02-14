/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 20:17:44 by chris             #+#    #+#             */
/*   Updated: 2024/02/14 14:03:25 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_minishell(t_common *c, char **envp)
{
	ft_memset(c, 0, sizeof(t_common));
	if(envp)
	{
		if (!dup_env(c, envp))
			return (perror("Error initializing environment\n"));
	}
	else
		return (perror("Error initializing environment\n"));
}

char	*prompt(void)
{
	char	*line;

	line = readline("minishell🔮: 🚬🦦❯ "); // check rl_redisplay
	if (ft_strlen(line) > 0)
		add_history(line);
	return (line);
}

void	init_loop_data(t_common *c){
	c->raw_prompt = NULL;
	c->prompt = NULL;
}

int	check_cmd(char *cmd, t_node *tokenslist)
{
	int		i;
	int		size;

	i = 0;
	size = ft_strlen(tokenslist->str[0]);
	while (cmd[i] && cmd[i] != ' ')
		i++;
	if (ft_strncmp(cmd, tokenslist->str[0], size) || i != size)
		return (0);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_common	c;
	//Validate size before allocation. The size variable
	// should be validated or bounded checked before allocating memory to avoid potential integer overflows.
	(void)		ac;
	(void)		av;
	
	init_minishell(&c, envp);
	c.tokenslist = malloc(sizeof(t_node *));
	if (!c.tokenslist)
		return (perror("Error initializing tokenslist\n"), 1);
	while (1)
	{
		init_loop_data(&c);
		c.raw_prompt = prompt();
		if (c.raw_prompt[0])
		{
			c.tokenslist->str = ft_split(c.raw_prompt, ' ');
			if (!c.tokenslist->str)
				return (perror("Error initializing tokenslist\n"), 1);
			if (check_cmd("pwd", c.tokenslist))
				ft_pwd();
			if (check_cmd("export", c.tokenslist))	
				ft_export(c.tokenslist->str, c.env);
			if (check_cmd("env", c.tokenslist))
				ft_env(c.env);
			if (check_cmd("exit", c.tokenslist))
				ft_exit(&c, c.tokenslist->str);
			if (check_cmd("unset", c.tokenslist))
				ft_unset(c.tokenslist->str, &c);
			if (check_cmd("echo", c.tokenslist))
				ft_echo(c.tokenslist->str);
			if (check_cmd("cd", c.tokenslist))
				ft_cd(c.tokenslist->str, &c);
			free_2d(c.tokenslist->str);
			free(c.raw_prompt);
		}
	}
	free_all(&c);
	return (0);
}
