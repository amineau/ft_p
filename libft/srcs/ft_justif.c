/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_justif.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 17:06:32 by amineau           #+#    #+#             */
/*   Updated: 2016/03/20 11:52:40 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_wh(char c, int n)
{
	char	*str;
	int		i;

	str = ft_strnew(n + 1);
	i = 0;
	while (i != n)
		str[i++] = c;
	str[i] = '\0';
	return (str);
}

char	*ft_sign(char *str, char s)
{
	char	*sign;

	if (str[0] != '-' && s)
	{
		sign = ft_strnew(1);
		sign[0] = s;
		str = ft_strclnjoin(sign, str);
	}
	return (str);
}

char	*ft_precision(char *str, int prec)
{
	int		t;
	char	*dest;

	t = (int)ft_strlen(str);
	if ((prec <= t && str[0] != '-') || prec < t)
	{
		if ((ft_strcmp(str, "0") == 0 || ft_strcmp(str, "-0") == 0) &&
				prec == 0)
			str[0] = '\0';
		return (str);
	}
	if (str[0] == '-')
	{
		dest = ft_strcln1join(ft_wh('0', prec - t + 1), &str[1]);
		ft_strdel(&str);
		return (ft_strcln2join("-", dest));
	}
	return (ft_strclnjoin(ft_wh('0', prec - t), str));
}

char	*ft_justif(char *str, int just, char f)
{
	int		t;
	char	*ptr;

	t = (int)ft_strlen(str);
	if (just <= t)
		return (str);
	if (f == '0' && (ft_strchr("-+ ", str[0])))
	{
		ptr = ft_strnew(1);
		ptr[0] = str[0];
		return (ft_strjoin(ft_strclnjoin(ptr, ft_wh(f, just - t)), &str[1]));
	}
	if (f == ' ' || f == '0')
		return (ft_strclnjoin(ft_wh(f, just - t), str));
	else
		return (ft_strclnjoin(str, ft_wh(' ', just - t)));
}

char	*ft_justif_string(char *str, size_t just, char f)
{
	size_t	t;

	t = ft_strlen(str);
	if (just <= t)
		return (str);
	if (f == ' ' || f == '0')
		return (ft_strclnjoin(ft_wh(f, just - t), str));
	else
		return (ft_strclnjoin(str, ft_wh(' ', just - t)));
}
