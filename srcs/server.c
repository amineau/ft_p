/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2018/08/15 22:41:08 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void	usage(char *str)
{
	ft_printf("Usage: %s <port> [-d directory path]\n", str);
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
		if (errno == EACCES)
			ft_printf("This address is protected\n");
		else if(errno == EADDRINUSE)
			ft_printf("This address is already in use\n");
		else
			ft_printf("Bind failed");
		exit(-1);
	}
	if (listen(sock, MAX_PENDING_CONNECTIONS) == -1)
	{
		if (errno == ECONNREFUSED)
			ft_printf("The queue is full");
		else
			ft_printf("Listen failed");
		exit(-1);
	}
	return(sock);
}

int		open_client(int sock)
{
	int					cs;
	struct sockaddr_in	csin;
	unsigned int		cslen;

	if ((cs = accept(sock, (struct sockaddr *)&csin, &cslen)) == -1)
	{
		if (errno == EBADF)
			ft_printf("The file descriptor is invalid\n");
		else if (errno == ECONNABORTED)
			ft_printf("The connection has been aborted\n");
		else
			ft_printf("Accept failed : %s\n", strerror(errno));
		exit(-1);
	}
	return (cs);
}

size_t	lexer(char ***tokens, const char *buff)
{
	size_t	i;

	i = 0;
	*tokens = ft_strsplit(buff, ' ');
	while (tokens[i++])
		;
	return i;
}

void	listen_clients(int sock)
{
	int		cs;
	int		r;
	char	buff[1024];
	pid_t	pid;
	char	**tokens;

	while(1)
	{
		cs = open_client(sock);
		ft_printf("socket : %d\nclient socket : %d\n", sock, cs);
		pid = fork();
		while((r = recv(cs, buff, 1023, 0)) > 0)
		{
			buff[r] = '\0';
			lexer(&tokens, buff);
			ft_printf("received %d bytes on pid %d : [%s]\n", r, pid, buff);
			ft_printf("tokens [0] : %s\n", tokens[0]);
		}
		ft_printf("%s\n", strerror(errno));
		close(cs);
	}
}

int		main(int ac, char **av)
{
	int port;
	int sock;

	if (ac < 2)
		usage(av[0]);
	port = ft_atoi(av[1]);
	sock = create_server(port);
	listen_clients(sock);
	close(sock);
	return(0);
}
