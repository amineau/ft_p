/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrayjoin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 19:53:26 by amineau           #+#    #+#             */
/*   Updated: 2018/08/16 21:21:56 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	cnt_char(char **s)
{
	size_t	cnt;

	cnt = 0;
	while (*s)
	{
		while(**s)
		{
			cnt++;
			(**s)++;
		}
		(*s)++;
	}
	return cnt;
}

char	*ft_arrayjoin(char **s)
{
	char	*dst;
	int		i;

	i = 0;
	if (!(dst = (char*)malloc(sizeof(char) * cnt_char(s) + 1)))
		return (NULL);
	while (*s)
	{
		while(**s)
			dst[i++] = (**s)++;
		(*s)++;
	}
	dst[i] = '\0';
	return (dst);
}
