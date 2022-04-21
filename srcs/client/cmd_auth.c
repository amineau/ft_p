/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_auth.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 23:41:05 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 13:14:35 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int ftp_cli_cmd_logout(t_cli_ftp *cli_ftp, const char *args)
{
	(void)cli_ftp;
	if (args)
	{
		put_no_req_arg(LOGOUT);
		return 0;
	}
	exit(EXIT_SUCCESS);
}

int ftp_cli_cmd_auth(t_cli_ftp *cli_ftp)
{
	t_server_verbs *srv_verbs;

	ftp_cli_send_pi(cli_ftp, AUTH_METHOD, "TLS");
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state == POS_DEF)
	{
		cli_ftp->pi.ssl = SSL_new(*cli_ftp->ctx);
		SSL_set_fd(cli_ftp->pi.ssl, cli_ftp->pi.sock);

		if (SSL_connect(cli_ftp->pi.ssl) <= 0)
		{
			printf("SSL_connect failed\n");
			exit(EXIT_FAILURE);
		}
		// printf("Connected with %s encryption\n", SSL_get_cipher(cli_ftp->pi.ssl));
		// ShowCerts(cli_ftp->pi.ssl);
		cli_ftp->pi.ssl_activated = true;
		return 1;
	}
	return 0;
}

int ftp_cli_cmd_password(t_cli_ftp *cli_ftp, t_client_args *args)
{
	t_server_verbs *srv_verbs;

	ftp_cli_send_pi(cli_ftp, PASSWORD, args->ca_pass);
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state == NEG_TMP)
		exit(EXIT_FAILURE_RETRY);
	else if (srv_verbs->sr_state != POS_DEF)
		exit(EXIT_FAILURE);
	return 1;
}

int ftp_cli_cmd_user(t_cli_ftp *cli_ftp, t_client_args *args)
{
	t_server_verbs *srv_verbs;

	ftp_cli_send_pi(cli_ftp, USERNAME, args->ca_user);
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state == POS_INT)
		ftp_cli_cmd_password(cli_ftp, args);
	else if (srv_verbs->sr_state == NEG_TMP)
		exit(EXIT_FAILURE_RETRY);
	else if (srv_verbs->sr_state == NEG_DEF)
		exit(EXIT_FAILURE);
	return 1;
}

int ftp_cli_cmd_protection_buffer_size(t_cli_ftp *cli_ftp)
{
	t_server_verbs *srv_verbs;

	ftp_cli_send_pi(cli_ftp, PROTECTION_BUFFER_SIZE, "0");
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state == NEG_TMP)
		exit(EXIT_FAILURE_RETRY);
	else if (srv_verbs->sr_state != POS_DEF)
		exit(EXIT_FAILURE);
	return 1;
}

int ftp_cli_cmd_protection(t_cli_ftp *cli_ftp)
{
	t_server_verbs *srv_verbs;

	ftp_cli_send_pi(cli_ftp, PROTECTION, "P");
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state == POS_DEF)
		cli_ftp->dtp.ssl_activated = true;
	else if (srv_verbs->sr_state == NEG_TMP)
		exit(EXIT_FAILURE_RETRY);
	else
		exit(EXIT_FAILURE);
	return 1;
}
