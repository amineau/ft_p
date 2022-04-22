/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_straddc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/26 04:20:30 by amineau           #+#    #+#             */
/*   Updated: 2022/04/19 10:19:53 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char *ft_straddc(const char *str, char c)
{
	char *dest;
	int   i;

	i = ft_strlen(str);
	if (!(dest = (char *)ft_memalloc(sizeof(char *) * (i + 1))))
		return (NULL);
	dest = ft_strcpy(dest, str);
	dest[i] = (char)c;
	dest[i + 1] = '\0';
	return (dest);
}
