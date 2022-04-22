/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncasecmp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/22 17:54:40 by amineau           #+#    #+#             */
/*   Updated: 2022/04/19 22:24:08 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_strncasecmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	i = 0;
	while (((ft_islower(s1[i]) && s1[i] == s2[i] + 32) ||
			(ft_isupper(s1[i]) && s1[i] == s2[i] - 32) || (s1[i] == s2[i])) &&
		   (s1[i] || s2[i]) && i + 1 < n)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
