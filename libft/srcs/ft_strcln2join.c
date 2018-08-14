/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcln2join.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/02 14:24:06 by amineau           #+#    #+#             */
/*   Updated: 2016/03/02 14:24:57 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcln2join(char const *s1, char const *s2)
{
	char	*dst;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!(dst = (char*)malloc(sizeof(char)
					* (ft_strlen(s1) + ft_strlen(s2) + 1))))
		return (NULL);
	if (s1)
	{
		i--;
		while (s1[++i])
			dst[i] = s1[i];
	}
	if (s2)
	{
		j--;
		while (s2[++j])
			dst[i + j] = s2[j];
		ft_strdel((char**)&s2);
	}
	dst[i + j] = '\0';
	return (dst);
}
