/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_integer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 17:34:52 by amineau           #+#    #+#             */
/*   Updated: 2016/03/20 17:02:48 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_char(t_format *lst, va_list ap)
{
	int		size;
	char	c;
	char	*str;

	if (lst->type != 'c')
		c = lst->type;
	else
		c = va_arg(ap, unsigned int);
	size = (lst->width <= 1) ? 0 : lst->width - 1;
	if (lst->just == '-')
	{
		ft_putchar(c);
		ft_putstr(str = ft_wh(' ', size));
	}
	else
	{
		ft_putstr(str = ft_wh(lst->just, size));
		ft_putchar(c);
	}
	size = ft_strlen(str) + 1;
	ft_strdel(&str);
	return (size);
}

int		ft_string(t_format *lst, va_list ap)
{
	char	*str;
	int		size;

	if (!ap || !(str = va_arg(ap, char*)))
		str = "(null)";
	size = (lst->precision >= 0) ? lst->precision : (int)ft_strlen(str);
	str = ft_justif_string(ft_strndup(str, size), lst->width, lst->just);
	ft_putstr(str);
	size = ft_strlen(str);
	ft_strdel(&str);
	return (size);
}

int		ft_int(t_format *lst, va_list ap)
{
	char	*str;
	int		size;

	ft_lenght_type(lst->type, &(lst->lenght));
	str = ft_precision(ft_signed_size(lst, 10, ap), lst->precision);
	str = ft_sign(str, lst->sign);
	str = ft_justif(str, lst->width, lst->just);
	ft_putstr(str);
	size = ft_strlen(str);
	ft_strdel(&str);
	return (size);
}

int		ft_unint(t_format *lst, va_list ap)
{
	char	*str;
	int		size;

	ft_lenght_type(lst->type, &(lst->lenght));
	str = ft_precision(ft_unsigned_size(lst, 10, ap), lst->precision);
	str = ft_justif(str, lst->width, lst->just);
	ft_putstr(str);
	size = ft_strlen(str);
	ft_strdel(&str);
	return (size);
}

int		ft_adrpoint(t_format *lst, va_list ap)
{
	char	*str;
	char	*dest;
	int		size;

	dest = ft_straddc(ft_strdup("0"), lst->type + 8);
	str = ft_precision(ft_utoa_base(va_arg(ap, uintmax_t), 16, lst->type + 8),
			lst->precision);
	str = ft_zero_justif(str, lst, dest);
	ft_putstr(str);
	size = ft_strlen(str);
	ft_strdel(&str);
	return (size);
}
