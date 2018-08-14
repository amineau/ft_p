/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 08:39:02 by amineau           #+#    #+#             */
/*   Updated: 2015/11/30 11:26:32 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int c, int fd)
{
	if (c == -2147483648)
		ft_putstr_fd("-2147483648", fd);
	else
	{
		if (c < 0)
		{
			c = -c;
			ft_putchar_fd('-', fd);
		}
		if (c >= 10)
		{
			ft_putnbr_fd(c / 10, fd);
			ft_putnbr_fd(c % 10, fd);
		}
		else
			ft_putchar_fd(c + 48, fd);
	}
}
