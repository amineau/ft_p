/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2018/08/15 01:36:02 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void	usage(char *str)
{
	printf("Usage: %s <host> <port>\n", str);
	exit(-1);
}

struct in_addr	htoaddr(char *name)
{
	struct hostent	*host;
	struct in_addr	addr;

	host = gethostbyname(name);
	if (!host)
	{
		if (h_errno == HOST_NOT_FOUND)
			ft_printf("%s is unknown\n", name);
		else if (h_errno == NO_DATA)
			ft_printf("%s does not have an IP address\n", name);
		else if (h_errno == NO_RECOVERY)
			ft_printf("Server error\n");
		else if (h_errno == TRY_AGAIN)
			ft_printf("A temporary error occurred on an authoritative name server.  Try again later.\n");
		exit(-1);
	}
	ft_memcpy(&addr.s_addr, host->h_addr, host->h_length);
	return(addr);
}

int		create_client(char *host, int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (!proto)
		exit(-1);
	sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr = htoaddr(host);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		if (errno == EADDRINUSE)
			ft_printf("Local address is already in use\n");
		else if (errno == ECONNREFUSED)
			ft_printf("Remote address not listening\n");
		else
			ft_printf("Connect error\n");
		exit(-1);
	}
	return(sock);
}


int		main(int ac, char **av)
{
	int port;
	int sock;
	char	*host;


	if (ac != 3)
		usage(av[0]);
	port = ft_atoi(av[2]);
	host = av[1];
	sock = create_client(host , port);
	write(sock, "Client connected\n", 17);
	close(sock);
	return(0);
}
