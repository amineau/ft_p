/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_zero_justif.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 17:19:05 by amineau           #+#    #+#             */
/*   Updated: 2016/03/17 17:22:00 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_zero_justif(char *str, t_format *lst, char *dest)
{
	if (lst->just == '0')
	{
		str = ft_justif(str, lst->width - 2, lst->just);
		str = ft_strclnjoin(dest, str);
	}
	else
	{
		str = ft_strclnjoin(dest, str);
		str = ft_justif(str, lst->width, lst->just);
	}
	return (str);
}
