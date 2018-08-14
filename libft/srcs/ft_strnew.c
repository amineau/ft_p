/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 08:57:08 by amineau           #+#    #+#             */
/*   Updated: 2015/11/29 09:44:52 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	char *dst;

	if (!(dst = (char*)malloc(sizeof(*dst) * (size + 1))))
		return (NULL);
	dst[size] = '\0';
	ft_strclr(dst);
	return (dst);
}
