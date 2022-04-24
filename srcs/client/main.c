/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 01:59:56 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_bool debug;

void usage(char *str)
{
	ft_printf("Usage: %s [-h <host>] [-p <port>] [-u <user name> [-w "
			  "<passwd>]] "
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
	t_client_action cmd_builtin[] = {ftp_cli_cmd_local_list,
									 ftp_cli_cmd_local_change_directory,
									 ftp_cli_cmd_local_print_directory,
									 ftp_cli_cmd_list,
									 ftp_cli_cmd_change_workdir,
									 ftp_cli_cmd_get_file,
									 ftp_cli_cmd_put_file,
									 ftp_cli_cmd_print_workdir,
									 ftp_cli_cmd_logout,
									 ftp_cli_cmd_help};
	return (cmd_builtin[cv->cv_code](cli_ftp, cv->cv_arg));
}

t_cli_ftp *ftp_cli_ftp_init()
{
	t_cli_ftp *cli_ftp;

	cli_ftp = (t_cli_ftp *)malloc(sizeof(t_cli_ftp));

	cli_ftp->pi.sock = 0;
	cli_ftp->pi.ssl = NULL;
	cli_ftp->pi.ssl_activated = false;
	cli_ftp->dtp.sock = 0;
	cli_ftp->dtp.ssl = NULL;
	cli_ftp->dtp.ssl_activated = false;
	cli_ftp->pwd = "/";
	return (cli_ftp);
}

void ftp_print_prompt(char *user, char *pwd)
{
	ft_printf("ftp://%s:%s> ", user, pwd);
}

void ftp_listen_user(t_cli_ftp *cli_ftp, char *user)
{
	t_client_verbs *cv;
	char           *buff;

	ftp_print_prompt(user, cli_ftp->pwd);
	while (get_next_line(STDIN_FILENO, &buff) > 0)
	{
		if (!(cv = ftp_cli_user_lexer(buff)))
		{
			ftp_print_prompt(user, cli_ftp->pwd);
			continue;
		}
		user_parser(cli_ftp, cv);
		free(cv);
		ftp_print_prompt(user, cli_ftp->pwd);
	}
}

void ftp_cli_run(t_client_args ca)
{
	SSL_CTX   *ctx;
	t_cli_ftp *cli_ftp;
	// int        status;
	// int        pid;

	ctx = ftp_cli_create_context();
	cli_ftp = ftp_cli_ftp_init();
	cli_ftp->ctx = &ctx;

	cli_ftp->pi.sin = ftp_get_socket_address(ca.ca_host, ca.ca_port);
	cli_ftp->pi.sock = ftp_create_socket();

	// if ((pid = fork()) != 0)
	// {
	ftp_connect_socket(cli_ftp->pi.sock, &cli_ftp->pi.sin);
	ftp_cli_connection_protocol(cli_ftp, &ca);
	ftp_listen_user(cli_ftp, ca.ca_user);
	// }
	// else
	// {
	// 	wait4(pid, &status, 0, 0);
	// 	ftp_free_ssl(cli_ftp);
	// 	if (!status || status != EXIT_FAILURE_RETRY)
	// 		return (status);
	// 	ft_printf("Unable to establish a connection to the server\n");
	// 	ft_printf("Wait for retry...\n");
	// 	sleep(5);
	// }
}

int main(int ac, char **av)
{
	t_client_args ca;

	getargs(ac, av, &ca);
	init_openssl();
	ftp_cli_run(ca);
	return (EXIT_SUCCESS);
}
