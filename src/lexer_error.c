/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:16:46 by miheider          #+#    #+#             */
/*   Updated: 2024/04/22 17:34:53 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	error_lexer(char *s, int i)
{
	if (*s == '>' || *s == '<' || *s == '|')
	{
		if (*s == '<' && i == 3)
			printf("❌ minishell: syntax error near unexpected token `<'\n");
		else if (*s == '<' && i > 3)
			printf("❌ minishell: syntax error near unexpected token `<<'\n");
		else if (*s == '>' && i == 3)
			printf("❌ minishell: syntax error near unexpected token `>'\n");
		else if (*s == '>' && i > 3)
			printf("❌ minishell: syntax error near unexpected token `>>'\n");
		else if (*s == '|' && (i == 2 || i == 3))
			printf ("❌ minishell: syntax error near unexpected token `|'\n");
		else if (*s == '|' && i > 3)
			printf ("❌ minishell: syntax error near unexpected token `||'\n");
		exit (2);
	}
}
