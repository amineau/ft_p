/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 12:06:20 by amineau           #+#    #+#             */
/*   Updated: 2015/12/02 11:32:07 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;
	char	*tmp;

	if (!b)
		return (NULL);
	i = 0;
	tmp = (char*)b;
	while (i < len)
		tmp[i++] = c;
	return ((unsigned char*)b);
}
