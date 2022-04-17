/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_sock.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2022/04/17 20:12:02 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_bool	is_valid_tls(char *str, size_t len)
{
	return ((!ft_strcmp(&str[len - 2], CRLF)));
}

char*	_received(t_srv_ftp *srv_ftp)
{
	char	buff[BUFF_SIZE];
	char*	ptr;
	char*	ret;
	int		r;

	if (srv_ftp->ssl_activated == true)
		r = SSL_read(srv_ftp->pi.ssl, buff, BUFF_SIZE - 1);
	else
		r = read(srv_ftp->pi.cs, buff, BUFF_SIZE - 1);
	ERR_print_errors_fp(stderr);
	if (r <=0)
		return (NULL);
	buff[r] = '\0';

	if (r != 0 && !is_valid_tls(buff, r))
	{
		ret = _received(srv_ftp);
		ft_strncat(buff, ret, BUFF_SIZE);
		ft_strdel(&ret);
	}
	ptr = (char*)malloc(sizeof(char) * (ft_strlen(buff) + 1));
	ft_strcpy(ptr, buff);
	return (ptr);
}

int		received(t_srv_ftp *srv_ftp, char* buff)
{
	char*	ret;

	buff[0] = '\0';
	ret = _received(srv_ftp);
	ft_strcpy(buff, ret);
	ft_strdel(&ret);
	return ft_strlen(buff);
}