/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:00:59 by caigner           #+#    #+#             */
/*   Updated: 2024/05/01 20:01:00 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*expand_str(t_common *c, char *str)
{
	int		i;
	int		varsize;
	char	*new;
	char	*envvalue;

	if (!str)
		return (NULL);
	i = 0;
	varsize = 0;
	while (str[i])
	{
		if (str[i] == '$' && !ft_strchr(WHITESPACE, str[i + 1]))
		{
			envvalue = get_expansion_value(c, str, i, &varsize);
			new = get_expanded_str(str, envvalue, i, varsize);
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
	expanded_str = expand_str(c, *str);
	free(*str);
	*str = expanded_str;
}
