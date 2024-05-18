/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 20:34:46 by miheider          #+#    #+#             */
/*   Updated: 2024/05/18 20:35:01 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	count_up(int *i, int *cc)
{
	(*i)++;
	(*cc)++;
}

int	add_token(t_common *c, char **value, int i, t_token **tmp)
{
	t_token	*token;
	int		ret;

	ret = 0;
	token = ft_protect(c, malloc(sizeof(t_token)), 0, 0);
	if (!value[i])
		return (free(token), -1);
	token->type = check_token(value[i]);
	if (token->type >= 1 && token->type <= 4)
	{
		if (value[i + 1])
			token->data = ft_protect(c, ft_strdup(value[i + 1]), 0, 0);
		else
		{
			printf("minishell❌: syntax error\n");
			free(token);
			return (-1);
		}
		ret = 1;
	}
	else
		token->data = ft_protect(c, ft_strdup(value[i]), 0, 0);
	*tmp = token;
	return (ret);
}

void	init_add_to_list(t_token **last, t_token **tmp, int *index)
{
	*index = 0;
	*last = NULL;
	*tmp = NULL;
}

void	add_to_list(t_common *c, char **token, t_list *lst)
{
	t_token	*tmp;
	t_token	*last;
	int		index;
	int		status;

	init_add_to_list(&last, &tmp, &index);
	while (token[index])
	{
		status = add_token(c, token, index, &tmp);
		if (status == 1)
			index += 2;
		else if (status <= 0)
			index++;
		if (last == NULL)
		{
			if (lst->content)
				free(lst->content);
			lst->content = tmp;
		}
		else
			last->next = tmp;
		last = tmp;
	}
	if (last != NULL)
		last->next = NULL;
}