/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@42>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:30:02 by miheider          #+#    #+#             */
/*   Updated: 2024/03/08 19:20:53 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void	rm_quotes(t_list_d *cmd_struct)
{
	t_cmd_table	*temp;
	size_t		cont_len;
	char		*new_content;
	int			i;

	temp = cmd_struct->content;
	while(temp != NULL)
	{
		if (cmd_struct->content != NULL && (temp->str[0][0] == '\'' || temp->str[0][0] == '"'))
		{
			cont_len = ft_strlen(cmd_struct->content);
			new_content = malloc(sizeof(char) * cont_len - 1);
			if (!new_content)
				return ;
			i = 0;
			while (i < cont_len - 1)
			{
				new_content[0][i] = temp->str[0][i + 1];
				i++;
			}
			new_content[cont_len - 2] = '\0';
			free(temp->str);
			temp->str = new_content;
		}
		temp = temp->next;
	}
}