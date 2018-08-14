/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_libft_wchar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/16 16:19:54 by amineau           #+#    #+#             */
/*   Updated: 2016/03/16 16:22:09 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

unsigned int	ft_wcharlen(wchar_t *w)
{
	unsigned int	i;

	i = 0;
	while (w[i])
		i++;
	return (i);
}

wchar_t			*ft_wcharndup(wchar_t *s1, size_t n)
{
	wchar_t	*s2;
	size_t	i;

	if (!(s2 = (wchar_t*)malloc(sizeof(wchar_t) * (ft_wcharlen(s1) + 1))))
		return (NULL);
	i = 0;
	while (s1[i] && i < n)
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
