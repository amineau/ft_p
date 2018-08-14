/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/25 12:57:28 by amineau           #+#    #+#             */
/*   Updated: 2016/03/21 15:59:16 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_printf(const char *format, ...)
{
	va_list		ap;
	int			size;
	char		*pourc;
	t_format	**lst;

	lst = (t_format**)ft_memalloc(sizeof(t_format*));
	va_start(ap, format);
	pourc = (char*)format;
	while (pourc)
	{
		if ((pourc = ft_strchr(pourc, '%')))
		{
			pourc++;
			if (pourc[0] != '%')
			{
				if (!ft_listadd_format(ft_listnew_format(pourc), lst))
					return (-1);
			}
			else
				pourc++;
		}
	}
	size = ft_algo(*lst, (char*)format, ap);
	ft_close_lst(lst, ap);
	return (size);
}
