/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_unsi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/26 06:08:17 by amineau           #+#    #+#             */
/*   Updated: 2016/03/17 18:07:08 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nbrdigit(uintmax_t nbr)
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

char		*ft_utoa(uintmax_t value)
{
	char	*str;
	int		pow;
	int		digit;
	int		i;

	i = 0;
	pow = nbrdigit(value);
	str = (char*)malloc(sizeof(char) * (pow + 1));
	while (--pow >= 0)
	{
		digit = value / ft_power(pow, 10);
		value %= ft_power(pow, 10);
		str[i++] = digit + 48;
	}
	str[i] = '\0';
	return (str);
}
