/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 19:04:39 by caigner           #+#    #+#             */
/*   Updated: 2023/12/04 16:47:39 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(int content)
{
	t_list	*node;

	node = (t_list *)ft_calloc(1, sizeof(t_list));
	if (!node)
	{
		return (0);
	}
	node->content = content;
	node->next = NULL;
	return (node);
}
