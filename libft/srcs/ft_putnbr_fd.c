/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 08:39:02 by amineau           #+#    #+#             */
/*   Updated: 2018/08/15 12:13:16 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_putnbr_fd(int fd, int c)
{
	if (c == -2147483648)
		ft_putstr_fd(fd, "-2147483648");
	else
	{
		if (c < 0)
		{
			c = -c;
			ft_putchar_fd(fd, '-');
		}
		if (c >= 10)
		{
			ft_putnbr_fd(fd, c / 10);
			ft_putnbr_fd(fd, c % 10);
		}
		else
			ft_putchar_fd(fd, c + 48);
	}
}
