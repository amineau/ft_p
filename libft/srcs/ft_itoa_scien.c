/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_scien.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 11:06:46 by amineau           #+#    #+#             */
/*   Updated: 2016/03/18 18:08:38 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	ft_test_zero(double nb)
{
	size_t	bit;

	bit = 0;
	while ((*(uintmax_t*)&nb & 1) == 0 && bit != 63)
	{
		*(uintmax_t*)&nb = *(uintmax_t*)&nb >> 1;
		bit++;
	}
	if (bit == 63)
		return (1);
	return (0);
}

char	*ft_itoa_scien(double nb, int prec, char e)
{
	char	*str;
	double	tmp;
	int		dec;

	if ((str = ft_nan_or_inf(nb, e)))
		return (str);
	dec = 0;
	tmp = (*(uintmax_t*)&nb >> 63 == 1) ? -nb : nb;
	str = (*(uintmax_t*)&nb >> 63 == 1) ? ft_strdup("-") : ft_strdup("");
	if (!ft_test_zero(nb))
	{
		while (tmp < 1 && dec-- <= 0)
			tmp *= 10;
		while (tmp >= 10)
		{
			tmp /= 10;
			dec++;
		}
	}
	str = ft_strclnjoin(str, ft_itoa_double(tmp, prec, e));
	str = ft_strclnjoin(str, ft_exposant(e, dec));
	return (str);
}
