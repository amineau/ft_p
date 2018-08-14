/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 15:17:26 by amineau           #+#    #+#             */
/*   Updated: 2015/12/02 11:33:57 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*t1;
	unsigned char	*t2;
	size_t			i;
	int				cmp;

	i = 0;
	cmp = 0;
	t1 = (unsigned char*)s1;
	t2 = (unsigned char*)s2;
	if (n == 0)
		return (0);
	while (cmp == 0 && i < n)
	{
		cmp = t1[i] - t2[i];
		i++;
	}
	return (cmp);
}
