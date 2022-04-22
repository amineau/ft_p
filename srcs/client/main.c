/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2022/04/22 21:52:49 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_bool debug;

void usage(char *str)
{
	printf("Usage: %s [-h <host>] [-p <port>] [-u <user name> [-w <passwd>]] "
		   "[-d <debug>] -\n",
		   str);
	exit(EXIT_FAILURE);
}

void getargs(int ac, char **av, struct s_client_args *ca)
{
	char opt;

	if (ac < 3)
		usage(av[0]);
	ca->ca_host = htoaddr("localhost");
	ca->ca_port = htons(21);
	ca->ca_user = ANONYMOUS_USER;
	ca->ca_pass = NULL;
	while ((opt = (char)getopt(ac, av, "hpuwd")) != -1)
	{
		if (opt == 'h')
			ca->ca_host = htoaddr(av[optind]);
		else if (opt == 'p')
			ca->ca_port = htons(ft_atoi(av[optind]));
		else if (opt == 'u')
			ca->ca_user = ft_strdup(av[optind]);
		else if (opt == 'w')
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
	t_client_action command[] = {ftp_cli_cmd_list,
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
		if (pid == 0)
		{
			cli_ftp.pi.sin = ftp_get_socket_address(ca.ca_host, ca.ca_port);
			cli_ftp.pi.sock = ftp_create_socket();
			cli_ftp.pi.ssl = false;
			cli_ftp.pi.ssl_activated = false;
			cli_ftp.dtp.ssl_activated = false;

			ftp_connect_socket(cli_ftp.pi.sock, &cli_ftp.pi.sin);
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
				return (status);
			printf("Unable to establish a connection to the server\n");
			printf("Wait for retry...\n");
			sleep(5);
		}
	}
	return (0);
}
