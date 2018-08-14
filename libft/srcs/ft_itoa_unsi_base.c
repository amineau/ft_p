/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_unsi_base.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/26 06:11:43 by amineau           #+#    #+#             */
/*   Updated: 2016/03/17 18:05:49 by amineau          ###   ########.fr       */
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

char		*ft_utoa_base(uintmax_t value, int base, char c)
{
	char	*str;
	int		pow;
	int		i;
	int		digit;

	i = 0;
	pow = nbrdigit(value, base);
	str = (char*)malloc(sizeof(char) * (pow + 1));
	while (--pow >= 0)
	{
		digit = value / ft_power(pow, base);
		value %= ft_power(pow, base);
		if (digit < 10)
			str[i++] = digit + 48;
		else
			str[i++] = digit + c - 33;
	}
	str[i] = '\0';
	return (str);
}
