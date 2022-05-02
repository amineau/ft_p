/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 20:08:29 by amineau           #+#    #+#             */
/*   Updated: 2022/04/28 14:37:39 by amineau          ###   ########.fr       */
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
		printf("sock: %d\nerrno: %d\n", sock, errno);
		error_print_exit(EXIT_FAILURE, err);
	}
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
}

int ftp_accept_connection(int sock, struct sockaddr_in *sin)
{
	int          cs;
	unsigned int cslen;
	char         str[INET_ADDRSTRLEN];

	cslen = sizeof(*sin);
	errno = 0;
	if ((cs = accept(sock, (struct sockaddr *)sin, &cslen)) == -1)
	{
		if (errno == EBADF)
			ft_printf("The file descriptor is invalid\n");
		else if (errno == ECONNABORTED)
			ft_printf("The connection has been aborted\n");
		else
			ft_printf("Accept failed\n\terrno : %d\n\terror : %s\n",
					  errno,
					  strerror(errno));
	}
	inet_ntop(AF_INET, &(sin->sin_addr), str, INET_ADDRSTRLEN);
	ft_printf("Connect to %s:%d\n", str, ntohs(sin->sin_port));
	return (cs);
}

int ftp_listen_connection(int sock)
{
	if (listen(sock, MAX_PENDING_CONNECTIONS) == -1)
	{
		if (errno == ECONNREFUSED)
			ft_printf("The queue is full");
		else
			ft_printf("Listen failed");
		return (-1);
	}
	return (1);
}
