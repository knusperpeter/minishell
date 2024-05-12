/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:00:59 by caigner           #+#    #+#             */
/*   Updated: 2024/05/12 21:40:47 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*expand_str_hd(t_common *c, char *str)
{
	int		i;
	int		varsize;
	char	*new;

	if (!str)
		return (NULL);
	i = 0;
	varsize = 0;
	while (str[i])
	{
		if (str[i] == '$' && !ft_strchr(WHITESPACE, str[i + 1]))
		{
			new = get_expanded_str(c, str, i, varsize);
			return (new);
		}
		i++;
	}
	return (NULL);
}

void	heredoc_expansion(t_common *c, t_io_red *io, char **str)
{
	char	*expanded_str;

	if (!io->expand_heredoc)
		return ;
	while (has_expansion(c, *str))
	{
		expanded_str = expand_str_hd(c, *str);
		free(*str);
		*str = expanded_str;
	}
}
