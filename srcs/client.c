/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2018/08/15 23:16:44 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void	usage(char *str)
{
	ft_printf("Usage: %s <host> <port> [-u <user name> [-p <passwd>]] -\n", str);
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

int		create_client(struct in_addr host, int port)
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
	sin.sin_addr = host;
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		if (errno == EADDRINUSE)
			ft_printf("Local address is already in use\n");
		else if (errno == ECONNREFUSED)
			ft_printf("Remote address not listening\n");
		else
			ft_printf("Connect failed\n");
		exit(-1);
	}
	ft_printf("Client connected\n");
	return(sock);
}

void	getargs(int ac, char** av, struct s_client_args *ca)
{
	char opt;

	if (ac < 3)
		usage(av[0]);
	ca->ca_host = htoaddr(av[1]);
	ca->ca_port = ft_atoi(av[2]);
	while((opt = (char)getopt(ac, av, "upd")) != -1)
	{
		if (opt == 'u')
			ca->ca_user = av[optind];
		else if (opt == 'p')
			ca->ca_pass = av[optind];
		else if (opt == 'd')
			ca->ca_dir = av[optind];
		else
			usage(av[0]);
	}
}

int		main(int ac, char **av)
{
	int		sock;
	char	*buff;
	int		gnllen;
	t_client_args	ca;

	getargs(ac, av, &ca);
	sock = create_client(ca.ca_host , ca.ca_port);
	while((gnllen = get_next_line(STDIN_FILENO, &buff)) > 0)
	{
		ft_printf_fd(sock, "Client send a message : %s\n", buff);
	}
	ft_printf("DEBUG : %s\n", strerror(errno));
	close(sock);
	return(0);
}
