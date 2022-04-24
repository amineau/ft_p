/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protocol.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 00:17:30 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 02:42:34 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

void ftp_init_protocol(t_cli_ftp *cli_ftp)
{
	t_server_verbs *srv_verbs;

	while ((srv_verbs = ftp_wait_for_response(cli_ftp)))
		if (srv_verbs->sr_state != POS_TMP)
			break;
	if (srv_verbs->sr_state == POS_DEF)
		return;
	else if (srv_verbs->sr_state == NEG_TMP)
		exit(EXIT_FAILURE_RETRY);
	exit(EXIT_FAILURE);
}

void ftp_cli_connection_protocol(t_cli_ftp *cli_ftp, t_client_args *ca)
{
	ftp_init_protocol(cli_ftp);
	ftp_cli_cmd_auth(cli_ftp);
	ftp_cli_cmd_user(cli_ftp, ca);
	if (cli_ftp->pi.ssl_activated)
	{
		ftp_cli_cmd_protection_buffer_size(cli_ftp);
		ftp_cli_cmd_protection(cli_ftp);
	}
	ftp_cli_cmd_passive_mode(cli_ftp);
}