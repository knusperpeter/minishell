/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 20:17:44 by chris             #+#    #+#             */
/*   Updated: 2024/02/07 22:36:53 by caigner          ###   ########.fr       */
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

	line = readline("🚬🦦🫗 ✗"); //$USER // check rl_redisplay
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
	char *x = "x=1";
	char *y	= "export";
	char **exportx = malloc(2*sizeof(char *));
	exportx[0] = y;
	exportx[1] = x;
	init_minishell(&c, envp);
	while (1)
	{
		init_loop_data(&c);
		c.raw_prompt = prompt();
		if (!ft_strncmp("pwd", c.raw_prompt, 3))
			ft_pwd();
		if (!ft_strncmp("export", c.raw_prompt, 6))
		{	
			ft_export(exportx, c.env);
			exportx[1] = NULL;
			ft_export(exportx, c.env);
		}
	}
	return (0);
}
