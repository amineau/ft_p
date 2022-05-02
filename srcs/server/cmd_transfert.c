/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_transfert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 16:07:54 by amineau           #+#    #+#             */
/*   Updated: 2022/05/02 20:31:35 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

t_srv_res cmd_port(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;
	int       i;
	char    **transfert_port_str;
	int       transfert_port[6];

	(void)srv_ftp;
	transfert_port_str = ft_strsplit(req->req_arg, ',');
	i = 0;
	while (transfert_port_str[i])
	{
		transfert_port[i] = ft_atoi(transfert_port_str[i]);
		i++;
	}
	srv_ftp->dtp.sin = ftp_get_socket_address(
		stoaddr(itoaddr(transfert_port)), htons(itoport(&(transfert_port[4]))));
	// srv_ftp->dtp.sock = ftp_create_socket();

	// ftp_bind_socket(srv_ftp->dtp.sock, &srv_ftp->dtp.sin);
	srv_ftp->conf.passive = 0;
	response = ftp_build_srv_res(_200, "Ok");
	return (response);
}

t_srv_res cmd_passive(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;
	socklen_t sinlen;
	char     *port;
	char     *ip;

	(void)req;
	// response = ftp_build_srv_res(_500, "Ok");
	// return (response);
	srv_ftp->dtp.sin = ftp_get_socket_address(stoaddr(htonl(INADDR_ANY)), 0);
	srv_ftp->dtp.sock = ftp_create_socket();
	ftp_bind_socket(srv_ftp->dtp.sock, &srv_ftp->dtp.sin);
	ftp_listen_connection(srv_ftp->dtp.sock);

	sinlen = sizeof(srv_ftp->dtp.sin);
	if (getsockname(
			srv_ftp->dtp.sock, (struct sockaddr *)&srv_ftp->dtp.sin, &sinlen) != 0)
		return (ftp_build_srv_res(_421, "Cannot open data connection."));
	ip = get_local_ip(srv_ftp->interface);
	port = porttostr(srv_ftp->dtp.sin.sin_port);
	srv_ftp->conf.passive = 1;
	response = ftp_build_srv_res(
		_227,
		ft_arrayjoin((char *[]){"Entering Passive Mode. ",
								ft_arraycjoin((char *[]){ip, port, NULL}, ','),
								NULL}));
	free(port);
	free(ip);
	return (response);
}
