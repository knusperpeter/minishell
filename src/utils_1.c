/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 20:33:37 by caigner           #+#    #+#             */
/*   Updated: 2024/05/12 00:40:24 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_list_d	*ft_lstnew_d(void *content)
{
	t_list_d	*new_node;

	new_node = (t_list_d *)malloc(sizeof(*new_node));
	if (new_node == NULL)
		return (NULL);
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->content = content;
	return (new_node);
}

t_list_d	*ft_lstlast_d(t_list_d *lst)
{
	t_list_d	*tmp;

	tmp = lst;
	if (!tmp)
		return (NULL);
	while (tmp)
	{
		if (tmp->next == NULL)
			return (tmp);
		tmp = tmp->next;
	}
	return (tmp);
}

void	ft_lst_d_add_back(t_list_d **lst, t_list_d *neu)
{
	t_list_d	*last;

	if (*lst == NULL)
		*lst = neu;
	else
	{
		last = ft_lstlast_d(*lst);
		last->next = neu;
		neu->prev = last;
	}
}

void	ft_lst_d_delone(t_list_d *lst, void (*del)(void *))
{
	if (lst == 0 || del == 0)
		return ;
	del(lst->content);
	free (lst);
}

void	ft_lst_d_clear(t_list_d **lst, void (*del)(void *))
{
	t_list_d	*save;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		save = (*lst)->next;
		ft_lst_d_delone (*lst, del);
		*lst = save;
	}
}
