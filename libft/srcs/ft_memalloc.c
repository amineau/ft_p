/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 10:34:52 by amineau           #+#    #+#             */
/*   Updated: 2015/12/02 11:34:16 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	size_t	i;
	char	*dst;

	if (!(dst = (void*)malloc(sizeof(*dst) * size)))
		return (NULL);
	i = 0;
	dst[size - 1] = '\0';
	while (i < size - 1)
		dst[i++] = '\0';
	return ((void*)dst);
}
