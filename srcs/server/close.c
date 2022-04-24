/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 13:29:55 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 13:30:08 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

void ftp_close_connection(t_srv_transfert *srv_transfert)
{
	char str[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &(srv_transfert->sin.sin_addr), str, INET_ADDRSTRLEN);
	ft_printf("Disconnect from %s:%d\n", str, ntohs(srv_transfert->sin.sin_port));
	if (srv_transfert->ssl)
		shutdown_ssl(srv_transfert->ssl);
	close(srv_transfert->cs);
}