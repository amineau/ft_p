/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 13:37:06 by amineau           #+#    #+#             */
/*   Updated: 2016/03/21 16:41:19 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nbrdigit(intmax_t nbr)
{
	int pow;

	pow = 0;
	while (nbr / 10 != 0)
	{
		nbr = nbr / 10;
		pow++;
	}
	return (pow + 1);
}

static int	bo(int digit, uintmax_t tmp, int i, char *str)
{
	str[i] = digit + 48;
	return (tmp);
}

char		*ft_itoa(intmax_t value)
{
	char		*str;
	int			neg;
	int			pow;
	int			i;
	intmax_t	tmp;

	i = 0;
	neg = (value < 0) ? 1 : 0;
	tmp = (value < 0) ? -value : value;
	pow = nbrdigit(tmp);
	str = (char*)malloc(sizeof(char) * (neg + pow + 1));
	if (neg == 1)
		str[i++] = '-';
	while (--pow >= 0)
		tmp = bo(tmp / ft_power(pow, 10),
				tmp % ft_power(pow, 10), i++, str);
	str[i] = '\0';
	return (str);
}
