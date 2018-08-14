/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_fonc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 11:12:31 by amineau           #+#    #+#             */
/*   Updated: 2016/03/17 19:40:40 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char		*ft_decimal(long double nb, int prec)
{
	char	*str;
	int		i;

	if (prec < 0)
		return (NULL);
	str = ft_strnew(prec + 1);
	i = 0;
	str[i++] = '.';
	nb = ft_arrondi_double(nb, prec);
	while (prec != 0)
	{
		nb *= 10;
		if (prec != 1)
			str[i++] = (int)nb + 48;
		else
			str[i++] = ft_arrondi(nb) + 48;
		nb = (nb - (int)nb);
		prec--;
	}
	str[i] = '\0';
	return (str);
}

char		*ft_integer(long double nb, char *str, int nbrdig, int prec)
{
	int	i;

	str = ft_strnew(nbrdig);
	i = 0;
	while (nbrdig != 0)
	{
		if (nbrdig != 1 || prec > 0)
			str[i++] = (int)nb + 48;
		else
			str[i++] = ft_arrondi(nb) + 48;
		nb = (nb - (int)nb) * 10;
		nbrdig--;
	}
	str[i] = '\0';
	return (str);
}

void		ft_refresh(char **str)
{
	int	i;

	i = (int)ft_strlen(*str) - 1;
	while (i >= 0 && (*str)[i] != '-')
	{
		if (!ft_isdigit((*str)[i]) && (*str)[i] != '.')
		{
			(*str)[i] = '0';
			if ((i != 0 && (*str)[0] != '-') || (i != 1 && (*str)[0] == '-'))
			{
				if ((*str)[i - 1] != '.')
					(*str)[i - 1]++;
				else
					(*str)[i - 2]++;
			}
			else if (i == 1)
			{
				(*str)[0] = '1';
				*str = ft_strcln2join("-", *str);
			}
			else
				*str = ft_strcln2join("1", *str);
		}
		i--;
	}
}

char		*ft_exposant(char e, int dec)
{
	char	*str;

	str = ft_strnew(2);
	str[0] = e;
	str[1] = (dec < 0) ? '-' : '+';
	if (ABS(dec) < 10)
		str = ft_strclnjoin(ft_straddc(str, '0'), ft_itoa(ABS(dec)));
	else
		str = ft_strclnjoin(str, ft_itoa(ABS(dec)));
	return (str);
}
