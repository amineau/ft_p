/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_double.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/28 11:28:40 by amineau           #+#    #+#             */
/*   Updated: 2016/03/20 14:28:28 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_charupper(char *str, char c)
{
	int i;

	str = ft_strdup(str);
	if (ft_isupper(c))
	{
		i = 0;
		while (str[i])
		{
			str[i] = ft_toupper(str[i]);
			i++;
		}
	}
	return (str);
}

char	*ft_nan_or_inf(double nb, char c)
{
	double			tmp;
	unsigned int	bit;

	if ((*(uintmax_t*)&nb >> 52) % 0x800 == 0x7FF)
	{
		tmp = nb;
		bit = 0;
		while ((*(uintmax_t*)&tmp & 1) == 0)
		{
			*(uintmax_t*)&tmp = *(uintmax_t*)&tmp >> 1;
			bit++;
		}
		if (bit == 52)
		{
			if (*(uintmax_t*)&nb >> 63 == 1)
				return (ft_charupper("-inf", c));
			else
				return (ft_charupper("inf", c));
		}
		else
			return (ft_charupper("nan", c));
	}
	return (NULL);
}

double	ft_recup(double dif, double nb, int i)
{
	int	j;

	j = 0;
	while (j++ < i)
		dif *= 10;
	return (ABS(nb - dif));
}

int		ft_nbrint(double *nb)
{
	int	nbrdig;

	nbrdig = 1;
	while (*nb > 10)
	{
		*nb /= 10;
		nbrdig++;
	}
	return (nbrdig);
}

char	*ft_itoa_double(double nb, int prec, char c)
{
	char	*str;
	double	tmp;
	int		i;

	if ((str = ft_nan_or_inf(nb, c)))
		return (str);
	tmp = (*(uintmax_t*)&nb >> 63 == 1) ? -nb : nb;
	str = (*(uintmax_t*)&nb >> 63 == 1) ? ft_strdup("-") : ft_strdup("");
	i = ft_nbrint(&tmp);
	while (i > 8)
	{
		i -= 8;
		str = ft_strclnjoin(str, ft_integer(tmp, str, 8, 1));
		tmp = ft_recup(ft_atof(str), nb, i);
		while (tmp > 10)
			tmp /= 10;
	}
	tmp = (prec < 1) ? ft_arrondi_double(tmp, i - 1) : tmp;
	str = ft_strclnjoin(str, ft_integer(tmp, str, i, prec));
	str = ft_strclnjoin(str, ft_decimal(ABS(nb - ft_atof(str)), prec));
	ft_refresh(&str);
	return (str);
}
