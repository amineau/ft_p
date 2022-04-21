/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_sock.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 23:38:28 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int get_next_line_wrapper(int fd, SSL *ssl, t_bool ssl_activated, char **buff)
{
	if (ssl_activated)
		return (get_next_line_ssl_eol(ssl, buff, FTP_EOC));
	return get_next_line_eol(fd, buff, FTP_EOC);
}