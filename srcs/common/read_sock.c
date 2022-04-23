/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_sock.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 00:15:57 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int get_next_line_wrapper(
	int fd, SSL *ssl, t_bool ssl_activated, char **buff, char *eol)
{
	if (ssl_activated)
		return (get_next_line_ssl_eol(ssl, buff, eol));
	return get_next_line_eol(fd, buff, eol);
}