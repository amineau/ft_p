/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 23:15:58 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_bool debug;

void usage(char *str)
{
	printf("Usage: %s <host> <port> [-u <user name> [-p <passwd>]] [-d <debug>] -\n", str);
	exit(EXIT_FAILURE);
}

int create_client(struct in_addr host, int port)
{
	int                sock;
	struct protoent   *proto;
	struct sockaddr_in sin;

	proto = getprotobyname("tcp");
	if (!proto)
		exit(EXIT_FAILURE);
	sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr = host;
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		if (errno == EADDRINUSE)
			printf("Local address is already in use\n");
		else if (errno == ECONNREFUSED)
			printf("Remote address not listening\n");
		else
			printf("Connect failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Client connected\n");
	return (sock);
}

void getargs(int ac, char **av, struct s_client_args *ca)
{
	char opt;

	if (ac < 3)
		usage(av[0]);
	ca->ca_host = htoaddr(av[1]);
	ca->ca_port = ft_atoi(av[2]);
	ca->ca_user = NULL;
	ca->ca_pass = NULL;
	while ((opt = (char)getopt(ac, av, "upd")) != -1)
	{
		if (opt == 'u')
			ca->ca_user = ft_strdup(av[optind]);
		else if (opt == 'p')
			ca->ca_pass = ft_strdup(av[optind]);
		else if (opt == 'd')
			debug = true;
		else
			usage(av[0]);
	}
}

void ftp_free_ssl(t_cli_ftp *cli_ftp)
{
	ft_printf("Client disconnected\n");
	if (cli_ftp->pi.ssl)
		SSL_free(cli_ftp->pi.ssl);
	if (cli_ftp->pi.sock)
		close(cli_ftp->pi.sock);
}

int user_parser(t_cli_ftp *cli_ftp, t_client_verbs *cv)
{
	t_client_action command[] = {
		// list,
		ftp_cli_cmd_change_workdir,
		// get_file,
		// put_file,
		ftp_client_cmd_print_workdir,
		ftp_cli_cmd_logout,
		ftp_cli_cmd_help};
	return (command[cv->cv_code](cli_ftp, cv->cv_arg));
}

int main(int ac, char **av)
{
	char          *buff;
	int            gnllen;
	t_client_args  ca;
	t_client_verbs cv;
	t_cli_ftp      cli_ftp;
	SSL_CTX       *ctx;
	int            pid;
	int            status;

	getargs(ac, av, &ca);

	init_openssl();
	ctx = ftp_cli_create_context();
	cli_ftp.ctx = &ctx;
	while (1)
	{
		pid = fork();
		if (pid != 0)
		{
			cli_ftp.pi.sock = create_client(ca.ca_host, ca.ca_port);
			cli_ftp.pi.ssl = false;
			cli_ftp.pi.ssl_activated = false;
			cli_ftp.dtp.ssl_activated = false;

			ftp_cli_connection_protocol(&cli_ftp, &ca);
			ft_putstr("$> ");
			while ((gnllen = get_next_line(STDIN_FILENO, &buff)) > 0)
			{
				if (ftp_cli_user_lexer(buff, &cv) == -1)
				{
					ft_putstr("$> ");
					continue;
				}
				user_parser(&cli_ftp, &cv);
				ft_putstr("$> ");
			}
		}
		else
		{
			wait4(pid, &status, 0, 0);
			ftp_free_ssl(&cli_ftp);
			if (!status || status != EXIT_FAILURE_RETRY)
			{
				break;
			}
			printf("Unable to establish a connection to the server\n");
			printf("Wait for retry...\n");
			sleep(5);
		}
	}
	return (0);
}
