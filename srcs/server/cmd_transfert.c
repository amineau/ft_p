/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_transfert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 16:07:54 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 14:25:47 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

t_srv_res cmd_port(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;
	int       i;
	char    **transfert_port_str;
	// int             transfert_port[6];

	(void)srv_ftp;
	transfert_port_str = ft_strsplit(req->req_arg, ',');
	i = 0;
	while (transfert_port_str[i])
	{
		// transfert_port[i] = ft_atoi(transfert_port_str[i]);
		i++;
	}
	response = ftp_build_srv_res(_200, "Ok");
	return (response);
}

char *addrtostr(in_addr_t s_addr)
{
	char **addr;
	char  *str;

	addr = (char *[]){
		ft_itoa((s_addr & 0x000000FF)),
		ft_itoa((s_addr & 0x0000FF00) >> 8),
		ft_itoa((s_addr & 0x00FF0000) >> 16),
		ft_itoa((s_addr & 0xFF000000) >> 24),
		NULL,
	};
	str = ft_arraycjoin(addr, ',');
	// ft_arrayfree(addr);
	return str;
}

char *porttostr(in_port_t sin_port)
{
	char **port;
	char  *str;

	port = (char *[]){
		ft_itoa((sin_port & 0x00FF)), ft_itoa((sin_port & 0xFF00) >> 8), NULL};

	str = ft_arraycjoin(port, ',');
	return str;
}

int ftp_create_sock(int port)
{
	int                sock;
	struct protoent   *proto;
	struct sockaddr_in sin;

	proto = getprotobyname("tcp");
	if (!proto)
		exit(EXIT_FAILURE);
	sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
	ft_bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		if (errno == EACCES)
			ft_printf("This address is protected\n");
		else if (errno == EADDRINUSE)
			ft_printf("This address is already in use\n");
		else
			ft_printf("Bind failed");
		exit(EXIT_FAILURE);
	}
	return sock;
}

int ftp_create_channel(int port)
{
	int sock;

	sock = ftp_create_sock(port);
	if (listen(sock, MAX_PENDING_CONNECTIONS) == -1)
	{
		if (errno == ECONNREFUSED)
			ft_printf("The queue is full");
		else
			ft_printf("Listen failed");
		return (-1);
	}
	return (sock);
}

t_srv_res cmd_passive_mode(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;
	socklen_t sinlen;
	char     *port;
	char     *addr;

	(void)req;
	srv_ftp->dtp.sin = ftp_get_socket_address(srv_ftp->pi.sin.sin_addr, htons(0));
	srv_ftp->dtp.sock = ftp_create_socket();
	ftp_bind_socket(srv_ftp->dtp.sock, &srv_ftp->dtp.sin);
	ftp_listen_connection(srv_ftp->dtp.sock);

	sinlen = sizeof(srv_ftp->dtp.sin);
	if (getsockname(
			srv_ftp->dtp.sock, (struct sockaddr *)&srv_ftp->dtp.sin, &sinlen) != 0)
		return (ftp_build_srv_res(_421, "Cannot open data connection."));
	addr = addrtostr(srv_ftp->dtp.sin.sin_addr.s_addr);
	port = porttostr(srv_ftp->dtp.sin.sin_port);
	response = ftp_build_srv_res(
		_227,
		ft_arrayjoin((char *[]){"Entering Passive Mode. ",
								ft_arraycjoin((char *[]){addr, port, NULL}, ','),
								NULL}));
	free(port);
	free(addr);
	return (response);
}
