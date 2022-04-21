/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstremoveelem.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/30 16:34:12 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 14:23:58 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_lstremoveelem(t_list **alst, t_list *elem)
{
	t_list *list;

	list = *alst;
	if (list == elem)
	{
		*alst = list->next;
		return;
	}
	while (list)
	{
		if (list->next == elem)
		{
			list->next = elem->next;
			return;
		}
		list = list->next;
	}
}
