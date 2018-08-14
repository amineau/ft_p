/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 13:35:57 by amineau           #+#    #+#             */
/*   Updated: 2015/12/02 11:33:30 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t		i;
	char		*tmp1;
	const char	*tmp2;

	if (!src)
		return (NULL);
	i = 0;
	tmp1 = dst;
	tmp2 = src;
	while (i < n && tmp2[i] != c)
	{
		tmp1[i] = tmp2[i];
		i++;
	}
	if (tmp2[i] == c && i != n)
	{
		tmp1[i] = c;
		return (ft_memchr(dst, c, n) + 1);
	}
	return (NULL);
}
