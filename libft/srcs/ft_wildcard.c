/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 16:07:04 by amineau           #+#    #+#             */
/*   Updated: 2016/03/17 18:02:46 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_wildcard(t_format *lst, va_list ap)
{
	if (lst->width == -1)
	{
		lst->width = va_arg(ap, int);
		if (lst->width < 0)
		{
			lst->width = -lst->width;
			lst->just = '-';
		}
	}
	else if (lst->wild_width)
		va_arg(ap, int);
	if (lst->precision == -1)
	{
		lst->precision = va_arg(ap, int);
		if (lst->precision < 0)
		{
			if (ft_strchr("eEfFgG", lst->type))
				lst->precision = 6;
			else if (ft_strchr("dDioOuUpxXbB", lst->type))
				lst->precision = 1;
		}
	}
}
