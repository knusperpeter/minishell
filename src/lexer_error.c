/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:16:46 by miheider          #+#    #+#             */
/*   Updated: 2024/04/27 13:56:42 by miheider         ###   ########.fr       */
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
		exit (2);
//	}
}
