/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strclnjoin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 16:36:01 by amineau           #+#    #+#             */
/*   Updated: 2016/02/28 20:31:51 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strclnjoin(char const *s1, char const *s2)
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
		ft_strdel((char**)&s1);
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
