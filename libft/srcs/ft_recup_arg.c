/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_recup_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 16:14:21 by amineau           #+#    #+#             */
/*   Updated: 2016/03/17 17:11:49 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_lenght_type(char c, char **str)
{
	if (ft_isupper(c) == 1)
	{
		if (!ft_strcmp(*str, "h"))
		{
			ft_strdel(str);
			*str = ft_strdup("l");
		}
		else
			*str = ft_strcln1join(*str, "l");
	}
	return (*str);
}

char	*ft_signed_size(t_format *lst, int base, va_list ap)
{
	if (ft_strcmp(lst->lenght, "hh") == 0)
		return (ft_itoa_base((char)va_arg(ap, int), base, lst->type));
	else if (ft_strcmp(lst->lenght, "h") == 0)
		return (ft_itoa_base((short)va_arg(ap, int), base, lst->type));
	else if (ft_strcmp(lst->lenght, "l") == 0)
		return (ft_itoa_base(va_arg(ap, long), base, lst->type));
	else if (ft_strcmp(lst->lenght, "ll") == 0 ||
			ft_strcmp(lst->lenght, "L") == 0)
		return (ft_itoa_base(va_arg(ap, long long), base, lst->type));
	else if (ft_strcmp(lst->lenght, "j") == 0)
		return (ft_itoa_base(va_arg(ap, intmax_t), base, lst->type));
	else if (ft_strcmp(lst->lenght, "z") == 0)
		return (ft_itoa_base(va_arg(ap, ssize_t), base, lst->type));
	else
		return (ft_itoa_base(va_arg(ap, int), base, lst->type));
}

char	*ft_unsigned_size(t_format *lst, int base, va_list ap)
{
	if (ft_strcmp(lst->lenght, "hh") == 0)
		return (ft_utoa_base((unsigned char)va_arg(ap, int),
					base, lst->type));
	else if (ft_strcmp(lst->lenght, "h") == 0)
		return (ft_utoa_base((unsigned short)va_arg(ap, int),
					base, lst->type));
	else if (ft_strcmp(lst->lenght, "l") == 0)
		return (ft_utoa_base(va_arg(ap, unsigned long), base, lst->type));
	else if (ft_strcmp(lst->lenght, "ll") == 0 ||
			ft_strcmp(lst->lenght, "L") == 0)
		return (ft_utoa_base(va_arg(ap, unsigned long long),
					base, lst->type));
	else if (ft_strcmp(lst->lenght, "j") == 0)
		return (ft_utoa_base(va_arg(ap, uintmax_t), base, lst->type));
	else if (ft_strcmp(lst->lenght, "z") == 0)
		return (ft_utoa_base(va_arg(ap, size_t), base, lst->type));
	else
		return (ft_utoa_base(va_arg(ap, unsigned int), base, lst->type));
}
