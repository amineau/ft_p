/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/25 12:57:28 by amineau           #+#    #+#             */
/*   Updated: 2018/08/15 13:24:02 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_vprintf_fd(int fd, const char *format, va_list ap)
{
	int        size;
	char      *pourc;
	t_format **lst;

	lst = (t_format **)ft_memalloc(sizeof(t_format *));
	pourc = (char *)format;
	while (pourc)
	{
		if ((pourc = ft_strchr(pourc, '%')))
		{
			pourc++;
			if (pourc[0] != '%')
			{
				if (!ft_listadd_format(ft_listnew_format(fd, pourc), lst))
					return (-1);
			}
			else
				pourc++;
		}
	}
	size = ft_algo(fd, *lst, (char *)format, ap);
	ft_close_lst(lst, ap);
	return (size);
}

int ft_printf_fd(int fd, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	return (ft_vprintf_fd(fd, format, ap));
}

int ft_printf(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	return (ft_vprintf_fd(STDOUT_FILENO, format, ap));
}