/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 20:08:29 by amineau           #+#    #+#             */
/*   Updated: 2022/04/23 09:46:48 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

struct sockaddr_in ftp_get_socket_address(struct in_addr addr, in_port_t port)
{
	struct sockaddr_in sin;

	ft_bzero((struct sockaddr *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = port;
	sin.sin_addr = addr;
	return (sin);
}

int ftp_create_socket()
{
	struct protoent *proto;
	int              sock;

	proto = getprotobyname("tcp");
	if (!proto)
		error_print_exit(EXIT_FAILURE, "Proto tcp has failed on create");
	if ((sock = socket(AF_INET, SOCK_STREAM, proto->p_proto)) == -1)
		error_print_exit(EXIT_FAILURE, "Socket has failed on create");
	return sock;
}

void ftp_connect_socket(int sock, struct sockaddr_in *sin)
{
	char *err;

	if (connect(sock, (const struct sockaddr *)sin, sizeof(*sin)) == -1)
	{
		if (errno == EADDRINUSE)
			err = "Local address is already in use";
		else if (errno == ECONNREFUSED)
			err = "Remote address not listening";
		else
			err = "Connect failed";
		error_print_exit(EXIT_FAILURE, err);
	}
	printf("Socket connected\n");
}

void ftp_bind_socket(int sock, struct sockaddr_in *sin)
{
	char *err;

	if (bind(sock, (const struct sockaddr *)sin, sizeof(*sin)) == -1)
	{
		if (errno == EACCES)
			err = "This address is protected";
		else if (errno == EADDRINUSE)
			err = "This address is already in use";
		else
			err = "Bind failed";
		error_print_exit(EXIT_FAILURE, err);
	}
	printf("Socket binded\n");
}

int ftp_accept_connection(int sock)
{
	int                cs;
	struct sockaddr_in csin;
	unsigned int       cslen;

	errno = 0;
	cslen = sizeof(csin);
	if ((cs = accept(sock, (struct sockaddr *)&csin, &cslen)) == -1)
	{
		if (errno == EBADF)
			printf("The file descriptor is invalid\n");
		else if (errno == ECONNABORTED)
			printf("The connection has been aborted\n");
		else
			printf("Accept failed\n\terrno : %d\n\terror : %s\n",
				   errno,
				   strerror(errno));
	}
	return (cs);
}

int ftp_listen_connection(int sock)
{
	if (listen(sock, MAX_PENDING_CONNECTIONS) == -1)
	{
		if (errno == ECONNREFUSED)
			printf("The queue is full");
		else
			printf("Listen failed");
		return (-1);
	}
	return (1);
}