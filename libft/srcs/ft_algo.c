/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_algo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/25 23:15:31 by amineau           #+#    #+#             */
/*   Updated: 2016/03/20 14:11:34 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_form2(t_format *tmp, va_list ap)
{
	if (tmp->type == 's')
		return (ft_string(tmp, ap));
	else if (tmp->type == 'C' || (tmp->type == 'c' &&
				ft_strcmp(tmp->lenght, "l") == 0))
		return (ft_wint(tmp, ap));
	else if (tmp->type == 'c')
		return (ft_char(tmp, ap));
	else if (tmp->type == 'd' || tmp->type == 'i' || tmp->type == 'D')
		return (ft_int(tmp, ap));
	else if (tmp->type == 'f' || tmp->type == 'F')
		return (ft_float(tmp, ap));
	else if (tmp->type == 'u' || tmp->type == 'U')
		return (ft_unint(tmp, ap));
	else if (tmp->type == 'o' || tmp->type == 'O')
		return (ft_octal(tmp, ap));
	else if (tmp->type == 'b' || tmp->type == 'B')
		return (ft_binaire(tmp, ap));
	else if (tmp->type == 'e' || tmp->type == 'E')
		return (ft_scienti(tmp, ap));
	else if (tmp->type == 'x' || tmp->type == 'X')
		return (ft_hexa(tmp, ap));
	else if (tmp->type == 'p')
		return (ft_adrpoint(tmp, ap));
	else
		return (ft_char(tmp, ap));
}

int		ft_form(t_format **lst, char **format, va_list ap)
{
	int			size;
	t_format	*tmp;

	tmp = *lst;
	size = 0;
	if (tmp->size != 1)
	{
		ft_wildcard(*lst, ap);
		if (tmp->type == 'S' || (tmp->type == 's' &&
					ft_strcmp(tmp->lenght, "l") == 0))
			size = ft_wchar(tmp, ap);
		else
			size = ft_form2(tmp, ap);
	}
	*format += tmp->size;
	*lst = tmp->next;
	ft_lstdel_format(tmp);
	return (size);
}

int		ft_algo(t_format *list, char *format, va_list ap)
{
	int	size;
	int	res;

	size = 0;
	while (*format)
	{
		if (*format != '%')
		{
			ft_putchar(*(format++));
			size++;
		}
		else if (format[1] == '%')
		{
			ft_putchar(format[1]);
			format += 2;
			size++;
		}
		else
		{
			if ((res = ft_form(&list, &format, ap)) == -1)
				return (-1);
			size += res;
		}
	}
	return (size);
}
