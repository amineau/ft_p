/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 20:31:04 by amineau           #+#    #+#             */
/*   Updated: 2015/12/01 16:45:07 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s)
{
	size_t	start;
	size_t	end;
	size_t	i;

	if (!s)
		return (NULL);
	start = 0;
	end = ft_strlen(s) - 1;
	while (s[start] && (s[start] == ' ' ||
				s[start] == '\t' || s[start] == '\n'))
		start++;
	i = start;
	end = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
			end = ++i;
		else
			i++;
	}
	if (end == 0)
		return (ft_strdup(""));
	return (ft_strsub(s, start, end - start));
}
