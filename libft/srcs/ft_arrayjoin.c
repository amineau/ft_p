/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrayjoin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 19:53:26 by amineau           #+#    #+#             */
/*   Updated: 2022/04/17 18:47:13 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t cnt_char(char **s)
{
	size_t cnt;
	int    i;

	cnt = 0;
	i = 0;
	while (s && s[i] != NULL)
		cnt += ft_strlen(s[i++]);
	return cnt;
}

char *ft_arrayjoin(char **s)
{
	char *dst;
	int   i;

	i = 0;
	if (!(dst = (char *)malloc(sizeof(char) * cnt_char(s) + 1)))
		return (NULL);
	while (s && *s != NULL)
	{
		while (**s)
			dst[i++] = *(*s)++;
		s++;
	}
	dst[i] = '\0';
	return (dst);
}
