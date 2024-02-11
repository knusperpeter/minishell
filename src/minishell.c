/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 20:17:44 by chris             #+#    #+#             */
/*   Updated: 2024/02/11 03:29:09 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_minishell(t_common *c, char **envp)
{
	ft_memset(c, 0, sizeof(t_common));
	if (!dup_env(c, envp))
		return (perror("Error initializing environment\n"));
}

char	*prompt(void)
{
	char	*line;

	line = readline("minishell🔮: 🚬🦦 ✗"); //$USER // check rl_redisplay
	if (ft_strlen(line) > 0)
		add_history(line);
	return (line);
}

void	init_loop_data(t_common *c){
	c->raw_prompt = NULL;
	c->prompt = NULL;
}
int	main(int ac, char **av, char **envp)
{
	t_common	c;
	int			size;
	(void)		ac;
	(void)		av;
	
	init_minishell(&c, envp);
	c.tokenslist = malloc(sizeof(t_node *));
	while (1)
	{
		init_loop_data(&c);
		c.raw_prompt = prompt();
		if (c.raw_prompt[0])
		{
			c.tokenslist->str = ft_split(c.raw_prompt, ' ');
			size = ft_strlen(c.tokenslist->str[0]);
			if (!ft_strncmp("pwd", c.tokenslist->str[0], size))
				ft_pwd();
			if (!ft_strncmp("export", c.tokenslist->str[0], size))	
				ft_export(c.tokenslist->str, c.env);
			if (!ft_strncmp("env", c.tokenslist->str[0], size))
				ft_env(c.env);
			if (!ft_strncmp("exit", c.tokenslist->str[0], size))
				ft_exit(&c, c.tokenslist->str);
			if (!ft_strncmp("unset", c.tokenslist->str[0], size))
				ft_unset(c.tokenslist->str, &c);
			if (!ft_strncmp("echo", c.tokenslist->str[0], size))
				ft_echo(c.tokenslist->str);
			free_2d(c.tokenslist->str);
			free(c.raw_prompt);
		}
	}
	free_all(&c);
	return (0);
}
