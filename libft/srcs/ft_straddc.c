/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_straddc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/26 04:20:30 by amineau           #+#    #+#             */
/*   Updated: 2016/03/03 14:20:19 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_straddc(char *str, char c)
{
	char	*dest;
	int		i;

	i = ft_strlen(str);
	if (!(dest = (char*)ft_memalloc(sizeof(char*) * (i + 1))))
		return (NULL);
	dest = ft_strcpy(dest, (const char*)str);
	dest[i] = (char)c;
	dest[i + 1] = '\0';
	if (str)
		ft_strdel(&str);
	return (dest);
}
