/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_floater.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 17:38:37 by amineau           #+#    #+#             */
/*   Updated: 2016/03/20 17:03:01 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_float(t_format *lst, va_list ap)
{
	char	*str;
	int		size;

	if (lst->precision == 0 && lst->conv != '#')
		lst->precision = -1;
	str = ft_itoa_double(va_arg(ap, double), lst->precision, lst->type);
	if (ft_strcmp(str, "nan") && ft_strcmp(str, "NAN"))
		str = ft_sign(str, lst->sign);
	str = ft_justif(str, lst->width, lst->just);
	ft_putstr(str);
	size = ft_strlen(str);
	ft_strdel(&str);
	return (size);
}

int		ft_scienti(t_format *lst, va_list ap)
{
	char	*str;
	int		size;

	if (lst->precision == 0 && lst->conv != '#')
		lst->precision = -1;
	str = ft_itoa_scien(va_arg(ap, double), lst->precision, lst->type);
	if (ft_isalpha(str[0]) && lst->just == '0')
		lst->just = ' ';
	if (ft_strcmp(str, "nan") && ft_strcmp(str, "NAN"))
		str = ft_sign(str, lst->sign);
	str = ft_justif(str, lst->width, lst->just);
	ft_putstr(str);
	size = ft_strlen(str);
	ft_strdel(&str);
	return (size);
}

int		ft_octal(t_format *lst, va_list ap)
{
	char	*str;
	char	*dest;
	int		size;

	dest = NULL;
	ft_lenght_type(lst->type, &(lst->lenght));
	lst->precision = (lst->conv == '#') ? lst->precision - 1 : lst->precision;
	str = ft_precision(ft_unsigned_size(lst, 8, ap), lst->precision);
	if (lst->conv == '#' && ft_strcmp(str, "0") != 0)
		dest = ft_strdup("0");
	str = ft_strclnjoin(dest, str);
	str = ft_justif(str, lst->width, lst->just);
	ft_putstr(str);
	size = ft_strlen(str);
	ft_strdel(&str);
	return (size);
}

int		ft_binaire(t_format *lst, va_list ap)
{
	char	*str;
	char	*dest;
	int		size;

	dest = NULL;
	ft_lenght_type(lst->type, &(lst->lenght));
	str = ft_precision(ft_unsigned_size(lst, 2, ap), lst->precision);
	if (lst->conv == '#' && ft_atoi(str) != 0)
	{
		dest = ft_strnew(2);
		dest[0] = '0';
		dest[1] = lst->type;
		str = ft_zero_justif(str, lst, dest);
	}
	else
	{
		str = ft_strclnjoin(dest, str);
		str = ft_justif(str, lst->width, lst->just);
	}
	ft_putstr(str);
	size = ft_strlen(str);
	ft_strdel(&str);
	return (size);
}

int		ft_hexa(t_format *lst, va_list ap)
{
	char	*str;
	char	*dest;
	int		size;

	dest = NULL;
	str = ft_precision(ft_unsigned_size(lst, 16, ap), lst->precision);
	if (lst->conv == '#' && ft_atoi_base(str, 16) != 0)
	{
		dest = ft_strnew(2);
		dest[0] = '0';
		dest[1] = lst->type;
		str = ft_zero_justif(str, lst, dest);
	}
	else
	{
		str = ft_strclnjoin(dest, str);
		str = ft_justif(str, lst->width, lst->just);
	}
	ft_putstr(str);
	size = ft_strlen(str);
	ft_strdel(&str);
	return (size);
}
