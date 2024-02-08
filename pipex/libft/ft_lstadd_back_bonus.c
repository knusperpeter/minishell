/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 19:55:24 by caigner           #+#    #+#             */
/*   Updated: 2023/12/16 12:06:21 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *newlst)
{
	t_list	*temp;

	temp = *lst;
	if (!newlst)
		return ;
	if (!*lst)
	{
		*lst = newlst;
		return ;
	}
	temp = ft_lstlast(temp);
	temp->next = newlst;
}
