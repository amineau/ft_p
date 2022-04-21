/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfindfirst.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/30 17:04:53 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 20:22:16 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list *ft_lstfindfirst(t_list *lst, void const *content, int (*f)(t_list *, void const *))
{
	if (!lst || !f)
		return (NULL);

	while (lst)
	{
		if (f(lst, content))
			return lst;
		lst = lst->next;
	}
	return NULL;
}
