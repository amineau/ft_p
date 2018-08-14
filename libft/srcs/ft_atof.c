/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 10:26:26 by amineau           #+#    #+#             */
/*   Updated: 2016/03/17 15:59:09 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

long double	ft_atof(char *str)
{
	long double	nb;
	int			size;
	int			i;

	size = ft_strlen(str);
	i = (str[0] == '-') ? 1 : 0;
	nb = 0;
	while (i < size)
		nb = nb * 10 + str[i++] - 48;
	nb = (str[0] == '-') ? -nb : nb;
	return (nb);
}
