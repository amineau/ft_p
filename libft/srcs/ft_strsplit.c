/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/28 09:51:33 by amineau           #+#    #+#             */
/*   Updated: 2015/12/01 11:07:28 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strlen_c(const char *s, char c)
{
	int i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static int	ft_cnt_word(const char *s, char c)
{
	int	i;
	int	cnt;

	i = 0;
	cnt = 0;
	while (s[i])
	{
		if ((i == 0 || s[i - 1] == c) && s[i] != c)
			cnt++;
		i++;
	}
	return (cnt);
}

static char	*ft_strcpy_c(char *dst, const char *src, char c)
{
	int		i;

	i = 0;
	while (src[i] && src[i] != c)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char		**ft_strsplit(char const *s, char c)
{
	char	**dst;
	int		i;
	int		h;

	i = 0;
	h = 0;
	if (!s || !(dst = (char**)malloc(sizeof(char*) * (ft_cnt_word(s, c) + 1))))
		return (NULL);
	dst[ft_cnt_word(s, c)] = (char*)'\0';
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			if (!(dst[h] = (char*)malloc(sizeof(char)
			* (ft_strlen_c(s + i, c) + 1))))
				return (NULL);
			ft_strcpy_c(dst[h], s + i, c);
			h++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (dst);
}
