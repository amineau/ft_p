/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arraycjoin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 19:53:26 by amineau           #+#    #+#             */
/*   Updated: 2022/04/17 18:38:40 by amineau          ###   ########.fr       */
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

static size_t cnt_size(char **s)
{
	size_t cnt;
	int    i;

	cnt = 0;
	i = 0;
	while (s[i++])
		cnt++;
	return cnt;
}

char *ft_arraycjoin(char **s, char c)
{
	char *dst;
	int   i;

	i = 0;
	if (!(dst = (char *)malloc(sizeof(char) * ((cnt_char(s) + cnt_size(s))))))
		return (NULL);
	while (s && *s != NULL)
	{
		while (**s)
		{
			dst[i++] = *(*s)++;
		}
		if (*(++s) != NULL)
			dst[i++] = c;
	}
	dst[i] = '\0';
	return (dst);
}
