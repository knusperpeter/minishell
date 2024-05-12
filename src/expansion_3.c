/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:53:24 by caigner           #+#    #+#             */
/*   Updated: 2024/05/13 01:29:45 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_split_cmd(t_common *c, t_list *curr)
{
	int		i;
	char	**tmp;
	t_list	*list;
	t_list	*new_list;
	t_list	*original_next;

	tmp = ft_protect(c, ft_split(curr->content, ' '), 0, 0);
	i = 0;
	list = curr;
	original_next = curr->next;
	free(list->content);
	list->content = tmp[i--];
	while (tmp[++i])
	{
		if (tmp[i + 1])
		{
			new_list = ft_protect(c, ft_lstnew(tmp[i + 1]), 0, 0);
			new_list->next = list->next;
			list->next = new_list;
			list = list->next;
		}
	}
	list->next = original_next;
	free(tmp);
}

int	split_command(t_list *lst)
{
	char	*str;
	int		last;

	if (!lst)
		return (0);
	str = lst->content;
	last = ft_strlen(str) - 1;
	if (last <= 0)
		return (0);
	if (ft_strchr("\'\"", str[0]) && ft_strchr("\'\"", str[last]))
		return (0);
	if (ft_strchr(lst->content, ' '))
		return (1);
	return (0);
}

int	check_split(t_common *c, t_cmd_table **t, t_list **a, t_list *b)
{
	char	*cmd;

	cmd = (*a)->content;
	if (!cmd[0] && b)
	{
		b->next = (*a)->next;
		free((*a)->content);
		free(*a);
		(*a) = b->next;
		return (1);
	}
	else if (!cmd[0] && !b)
	{
		(*t)->cmds = (*a)->next;
		free((*a)->content);
		free(*a);
		(*a) = (*t)->cmds;
		return (1);
	}
	if (*a)
		if (split_command(*a))
			ft_split_cmd(c, *a);
	return (0);
}
