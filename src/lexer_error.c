/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:16:46 by miheider          #+#    #+#             */
/*   Updated: 2024/05/06 18:50:24 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	error_lexer(char *s, int i)
{
/*	if (*s == '>' || *s == '<' || *s == '|')
	{
		if (*s == '<' && i == 3)
			ft_putstr_fd("❌ minishell: syntax error near unexpected token `<'\n", 2);
		else if (*s == '<' && i > 3)
			ft_putstr_fd("❌ minishell: syntax error near unexpected token `<<'\n", 2);
		else if (*s == '>' && i == 3)
			ft_putstr_fd("❌ minishell: syntax error near unexpected token `>'\n", 2);
		else if (*s == '>' && i > 3)
			ft_putstr_fd("❌ minishell: syntax error near unexpected token `>>'\n", 2);*/
		if (*s == '|' && (i == 2 || i == 3))
			ft_putstr_fd("❌ minishell: syntax error near unexpected token `|'\n", 2);
		else if (*s == '|' && i > 3)
			ft_putstr_fd("❌ minishell: syntax error near unexpected token `||'\n", 2);
		exit (2); //comment chris: not exit, but set exitstatus to 2!!
//	}
}
