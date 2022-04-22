/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_files_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 23:43:53 by amineau           #+#    #+#             */
/*   Updated: 2022/04/22 02:09:19 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int ftp_cli_cmd_list(t_cli_ftp *cli_ftp, const char *args)
{
	t_server_verbs *srv_verbs;
	int             ret;
	char           *buff;

	ftp_cli_send_pi(cli_ftp, LIST, args);
	if (connect(cli_ftp->dtp.sock, (const struct sockaddr *)&cli_ftp->dtp.sin, sizeof(cli_ftp->dtp.sin)) == -1)
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
	if (cli_ftp->dtp.ssl_activated == true)
	{
		cli_ftp->dtp.ssl = SSL_new(*cli_ftp->ctx);
		SSL_set_fd(cli_ftp->dtp.ssl, cli_ftp->dtp.sock);

		if (SSL_connect(cli_ftp->dtp.ssl) <= 0)
		{
			printf("SSL_connect failed\n");
			exit(EXIT_FAILURE);
		}
	}

	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state != POS_TMP)
		exit(EXIT_FAILURE_RETRY);
	while ((ret = get_next_line_wrapper(
				cli_ftp->dtp.sock,
				cli_ftp->dtp.ssl,
				cli_ftp->dtp.ssl_activated,
				&buff)) > 0)
		printf("%s\n", buff);
	if (cli_ftp->dtp.ssl_activated)
		shutdown_ssl(cli_ftp->dtp.ssl);
	close(cli_ftp->dtp.sock);
	if (ret == -1)
		exit(EXIT_FAILURE_RETRY);
	return 1;
}

int ftp_cli_cmd_change_workdir(t_cli_ftp *cli_ftp, const char *args)
{
	t_server_verbs *srv_verbs;

	if (!args)
	{
		put_req_arg(CHANGE_WORKDIR);
		return 0;
	}
	ftp_cli_send_pi(cli_ftp, CHANGE_WORKDIR, args);
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state == NEG_TMP || srv_verbs->sr_state == NEG_DEF)
	{
		ft_putstr(srv_verbs->user_info);
		return 0;
	}
	return 1;
}

// int		get_file(t_cli_ftp *cli_ftp, const char *args)
// {
// 	if (!cv->cv_arg)
// 	{
// 		put_req_arg(cv->cv_verb);
// 		return (NULL);
// 	}
// 	return (ft_strcjoin(RETRIEVE, cv->cv_arg, ' '));
// }

// int		put_file(t_cli_ftp *cli_ftp, const char *args)
// {
// 	if (!cv->cv_arg)
// 	{
// 		put_req_arg(cv->cv_verb);
// 		return (NULL);
// 	}
// 	return (ft_strcjoin(STORE, cv->cv_arg, ' '));
// }

int ftp_client_cmd_print_workdir(t_cli_ftp *cli_ftp, const char *args)
{
	t_server_verbs *srv_verbs;
	char          **split;

	if (args)
	{
		put_no_req_arg(PRINT_WORKDIR);
		return 0;
	}
	ftp_cli_send_pi(cli_ftp, PRINT_WORKDIR, args);
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state == NEG_TMP)
		exit(EXIT_FAILURE_RETRY);
	else if (srv_verbs->sr_state == NEG_DEF)
		exit(EXIT_FAILURE);
	split = ft_strsplit(srv_verbs->user_info, '"');
	printf("%s\n", split[0]);
	return 1;
}