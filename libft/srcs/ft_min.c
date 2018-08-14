/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_min.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/02 11:18:03 by amineau           #+#    #+#             */
/*   Updated: 2015/12/02 11:35:38 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_min(int *tab, size_t len)
{
	size_t	i;
	int		min;

	i = 1;
	if (len == 0)
		return (0);
	min = tab[0];
	while (i < len)
	{
		if (min > tab[i])
			min = tab[i];
		i++;
	}
	return (min);
}
