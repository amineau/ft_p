/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_power.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/26 02:57:38 by amineau           #+#    #+#             */
/*   Updated: 2016/02/26 08:06:35 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

uintmax_t	ft_power(int nbr, int base)
{
	int			i;
	uintmax_t	res;

	i = 0;
	res = 1;
	while (i++ < nbr)
		res *= base;
	return (res);
}
