/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2018/08/15 01:57:36 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void	usage(char *str)
{
	printf("Usage: %s <port>\n", str);
	exit(-1);
}

int		create_server(int port)
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
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		ft_printf("bind error");
		exit(-1);
	}
	listen(sock, 42);
	return(sock);
}

int		main(int ac, char **av)
{
	int port;
	int sock;
	int cs;
	struct sockaddr_in	csin;
	unsigned int		cslen;
	int r;
	char	buff[1024];
	pid_t	pid;


	if (ac != 2)
		usage(av[0]);
	port = ft_atoi(av[1]);
	sock = create_server(port);
	while(1)
	{
		cs = accept(sock, (struct sockaddr *)&csin, &cslen);
		pid = fork();
		while((r = read(cs, buff, 1023)) > 0)
		{
			buff[r] = '\0';
			ft_printf("received %d : [%s]\n", r, buff);
		}
		close(cs);
	}
	close(sock);
	return(0);
}
