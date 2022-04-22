/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 19:53:26 by amineau           #+#    #+#             */
/*   Updated: 2018/08/16 03:17:00 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strcjoin(char const *s1, char const *s2, char c)
{
	char *dst;
	int   i;
	int   j;

	i = 0;
	j = 0;
	if (!(dst = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 2))))
		return (NULL);
	if (s1)
	{
		i--;
		while (s1[++i])
			dst[i] = s1[i];
	}
	if (s1 && s2)
		dst[i++] = c;
	if (s2)
	{
		j--;
		while (s2[++j])
			dst[i + j] = s2[j];
	}
	dst[i + j] = '\0';
	return (dst);
}
