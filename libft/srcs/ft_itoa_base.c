/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/25 16:50:53 by amineau           #+#    #+#             */
/*   Updated: 2016/03/17 18:05:08 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nbrdigit(uintmax_t nbr, int base)
{
	int pow;

	pow = 0;
	while (nbr / base != 0)
	{
		nbr = nbr / base;
		pow++;
	}
	return (pow + 1);
}

static char	*asneg(intmax_t nb, int *i, int base)
{
	int			neg;
	char		*str;
	uintmax_t	tmp;

	neg = (nb < 0) ? 1 : 0;
	tmp = (nb < 0) ? -nb : nb;
	str = (char*)malloc(sizeof(char) * (neg + nbrdigit(tmp, base) + 1));
	if (neg == 1)
		str[(*i)++] = '-';
	return (str);
}

char		*ft_itoa_base(intmax_t value, int base, char c)
{
	char		*str;
	int			pow;
	int			i;
	int			digit;
	uintmax_t	tmp;

	i = 0;
	tmp = (value < 0) ? -value : value;
	pow = nbrdigit(tmp, base);
	str = asneg(value, &i, base);
	while (--pow >= 0)
	{
		digit = tmp / ft_power(pow, base);
		tmp %= ft_power(pow, base);
		if (digit < 10)
			str[i++] = digit + 48;
		else
			str[i++] = digit + c - 33;
	}
	str[i] = '\0';
	return (str);
}
