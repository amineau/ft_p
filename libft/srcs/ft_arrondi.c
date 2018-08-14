/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrondi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/26 10:17:09 by amineau           #+#    #+#             */
/*   Updated: 2016/03/16 16:11:02 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			ft_arrondi(long double nb)
{
	if (nb - (int)nb < 0.5)
		return ((int)nb);
	return ((int)(nb + 1));
}

long double	ft_arrondi_double(long double nb, size_t dec)
{
	long double tmp;
	int			neg;
	uintmax_t	pow;
	intmax_t	tmp_int;

	pow = ft_power(dec, 10);
	tmp = (nb < 0) ? -nb * pow : nb * pow;
	neg = (nb < 0) ? -1 : 1;
	tmp_int = tmp;
	if (tmp - (intmax_t)tmp < 0.5)
		return ((long double)tmp_int * neg / pow);
	return ((long double)(tmp_int + 1) * neg / pow);
}
