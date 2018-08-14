/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wchar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 17:41:19 by amineau           #+#    #+#             */
/*   Updated: 2016/03/20 14:27:11 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_wint(t_format *lst, va_list ap)
{
	wint_t	w;
	int		size;
	int		blanc;
	char	*str;

	w = va_arg(ap, wint_t);
	if ((size = ft_cntwint(w)) == -1)
		return (size);
	blanc = (lst->width <= size) ? 0 : lst->width - 1;
	if (lst->just == '-')
	{
		ft_putwint(w, size);
		ft_putstr(str = ft_wh(' ', blanc));
	}
	else
	{
		ft_putstr(str = ft_wh(lst->just, blanc));
		ft_putwint(w, size);
	}
	ft_strdel(&str);
	return (size);
}

int		ft_wchar(t_format *lst, va_list ap)
{
	wchar_t	*w;
	int		size;

	if (!(w = va_arg(ap, wchar_t*)))
		size = ft_string(lst, NULL);
	else
		size = ft_putwchar(w, lst);
	return (size);
}
