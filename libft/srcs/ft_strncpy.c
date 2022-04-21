/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 14:27:33 by amineau           #+#    #+#             */
/*   Updated: 2022/04/18 22:44:23 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dst, const char *src, size_t n)
{
	size_t i;

	i = 0;
	if (!src)
		return (dst);
	while (src[i] && i != n)
	{
		dst[i] = src[i];
		i++;
	}
	while (i < n)
		dst[i++] = '\0';
	dst[n] = '\0';
	return (dst);
}
