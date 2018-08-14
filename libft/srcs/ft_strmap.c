/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 19:09:27 by amineau           #+#    #+#             */
/*   Updated: 2015/11/30 10:39:01 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	char	*dst;
	int		i;

	if (!s || !(dst = (char*)malloc(sizeof(s))))
		return (NULL);
	i = 0;
	while (s[i])
	{
		dst[i] = f(s[i]);
		i++;
	}
	return (dst);
}
