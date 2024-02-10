/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 20:17:44 by chris             #+#    #+#             */
/*   Updated: 2024/02/10 19:55:46 by caigner          ###   ########.fr       */
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

	(void)		ac;
	(void)		av;
	//TEST CASE
	char **exportx = malloc(3*sizeof(char *));
	char **exitx = malloc(sizeof (char *));
	char **unsetx = malloc(3*sizeof (char *));
	exportx[0] = "export";
	exportx[1] = "x=1";
	exportx[2] = NULL;
	exitx[0] = "exit";
	unsetx[0] = "unset";
	unsetx[1] = "x";
	unsetx[2] = NULL;
	
	init_minishell(&c, envp);
	while (1)
	{
		init_loop_data(&c);
		c.raw_prompt = prompt();
		if (!ft_strncmp("pwd", c.raw_prompt, 3))
			ft_pwd();
		if (!ft_strncmp("export", c.raw_prompt, 6))	
			ft_export(exportx, c.env);
		if (!ft_strncmp("env", c.raw_prompt, 3))
			ft_env(c.env);
		if (!ft_strncmp("exit", c.raw_prompt, 4))
			ft_exit(&c, exitx);
		if (!ft_strncmp("unset", c.raw_prompt, 5))
			ft_unset(unsetx, &c);
	}
	free(exportx);
	return (0);
}
