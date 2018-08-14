/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putwint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/10 15:29:19 by amineau           #+#    #+#             */
/*   Updated: 2016/03/16 16:16:42 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		*ft_onebit(unsigned int w)
{
	int	*t;

	if (!(t = (int*)malloc(sizeof(int))))
		return (0);
	t[0] = w;
	return (t);
}

int		*ft_twobit(unsigned int w)
{
	int	*t;

	if (!(t = (int*)malloc(sizeof(int) * 2)))
		return (0);
	t[0] = 0XC0 + (w >> 6);
	t[1] = 0X80 + (w % 0X40);
	return (t);
}

int		*ft_threebit(unsigned int w)
{
	int	*t;

	if (!(t = (int*)malloc(sizeof(int) * 3)))
		return (0);
	t[0] = 0XE0 + (w >> 12);
	t[1] = 0X80 + ((w >> 6) % 0X40);
	t[2] = 0X80 + (w % 0X40);
	return (t);
}

int		*ft_fourbit(unsigned int w)
{
	int	*t;

	if (!(t = (int*)malloc(sizeof(int) * 4)))
		return (0);
	t[0] = 0XF0 + (w >> 18);
	t[1] = 0X80 + ((w >> 12) % 0X40);
	t[2] = 0X80 + ((w >> 6) % 0X40);
	t[3] = 0X80 + (w % 0X40);
	return (t);
}

void	ft_putwint(unsigned int w, int oc)
{
	int	*t;
	int	i;

	if (oc == 1)
		t = ft_onebit(w);
	else if (oc == 2)
		t = ft_twobit(w);
	else if (oc == 3)
		t = ft_threebit(w);
	else if (oc == 4)
		t = ft_fourbit(w);
	else
		return ;
	i = 0;
	while (i < oc)
		write(1, &t[i++], 1);
	ft_memdel((void**)&t);
}
