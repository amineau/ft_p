/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/25 15:01:06 by amineau           #+#    #+#             */
/*   Updated: 2016/02/25 15:03:30 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnchr(const char *s, int c, int n)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c && i < n)
		i++;
	if (s[i] == c)
		return ((char*)s + i);
	else
		return (NULL);
}
