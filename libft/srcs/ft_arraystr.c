/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arraystr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 19:53:26 by amineau           #+#    #+#             */
/*   Updated: 2018/08/16 15:37:25 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_arraystr(const char **array, const char* str)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (!ft_strcasecmp(array[i], str))
			return (i);
		i++;
	}
	return (-1);
}
