/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_files_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 23:43:53 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 02:42:34 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"
#include "ftp_srv_cmd_static.h"

int ftp_cli_cmd_list(t_cli_ftp *cli_ftp, const char *args)
{
	t_server_verbs *srv_verbs;
	int             ret;
	char           *buff;

	ftp_cli_send_pi(cli_ftp, cmd_str[LIST], args);
	cli_ftp->dtp.sock = ftp_create_socket();
	ftp_connect_socket(cli_ftp->dtp.sock, &cli_ftp->dtp.sin);
	if (cli_ftp->dtp.ssl_activated == true)
	{
		cli_ftp->dtp.ssl = ftp_create_ssl(cli_ftp->dtp.sock, *cli_ftp->ctx);
		ftp_connect_ssl(cli_ftp->dtp.ssl);
	}
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state != POS_TMP)
		exit(EXIT_FAILURE_RETRY);
	while ((ret = get_next_line_wrapper(cli_ftp->dtp.sock,
										cli_ftp->dtp.ssl,
										cli_ftp->dtp.ssl_activated,
										&buff,
										FTP_EOC)) > 0)
		ft_printf("%s\n", buff);
	if (cli_ftp->dtp.ssl_activated)
		shutdown_ssl(cli_ftp->dtp.ssl);
	close(cli_ftp->dtp.sock);
	if (ret == -1)
		error_print_exit(EXIT_FAILURE_RETRY, "Unable to parse dtp response");
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state != POS_DEF)
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
	ftp_cli_send_pi(cli_ftp, cmd_str[CHANGE_WORKDIR], args);
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state == NEG_TMP || srv_verbs->sr_state == NEG_DEF)
	{
		ft_putendl(srv_verbs->user_info);
		return 0;
	}
	ftp_cli_send_pi(cli_ftp, cmd_str[PRINT_WORKDIR], NULL);
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state == NEG_TMP)
		exit(EXIT_FAILURE_RETRY);
	else if (srv_verbs->sr_state == NEG_DEF)
		exit(EXIT_FAILURE);
	cli_ftp->pwd = ft_strsplit(srv_verbs->user_info, '"')[0];
	return 1;
}

int ftp_cli_cmd_get_file(t_cli_ftp *cli_ftp, const char *filename)
{
	t_server_verbs *srv_verbs;
	int             ret;
	char           *buff;
	int             fd;
	t_bool          is_opened;

	if (!filename)
	{
		put_no_req_arg(RETRIEVE);
		return (0);
	}
	// check if is a file before ask server
	ftp_cli_send_pi(cli_ftp, cmd_str[RETRIEVE], filename);
	cli_ftp->dtp.sock = ftp_create_socket();
	ftp_connect_socket(cli_ftp->dtp.sock, &cli_ftp->dtp.sin);
	if (cli_ftp->dtp.ssl_activated == true)
	{
		cli_ftp->dtp.ssl = ftp_create_ssl(cli_ftp->dtp.sock, *cli_ftp->ctx);
		ftp_connect_ssl(cli_ftp->dtp.ssl);
	}
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state != POS_TMP)
		exit(EXIT_FAILURE_RETRY);
	if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0770)) == -1)
	{
		ft_printf("Create or open file has failed\n");
		is_opened = false;
	}
	else
		is_opened = true;
	while ((ret = get_next_line_wrapper(cli_ftp->dtp.sock,
										cli_ftp->dtp.ssl,
										cli_ftp->dtp.ssl_activated,
										&buff,
										LF)) > 0)
	{
		if (is_opened)
		{
			ft_printf("SERVER-DTP: %s\n", buff);
			write(fd, buff, ft_strlen(buff));
			write(fd, CRLF, ft_strlen(CRLF));
		}
	}
	close(fd);

	if (cli_ftp->dtp.ssl_activated)
		shutdown_ssl(cli_ftp->dtp.ssl);
	close(cli_ftp->dtp.sock);
	if (ret == -1)
		error_print_exit(EXIT_FAILURE_RETRY, "Unable to parse dtp response");
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state != POS_DEF)
		exit(EXIT_FAILURE_RETRY);
	return 1;
}

int ftp_cli_cmd_put_file(t_cli_ftp *cli_ftp, const char *args)
{
	t_server_verbs *srv_verbs;

	if (!args)
	{
		put_no_req_arg(STORE);
		return (0);
	}
	ftp_cli_send_pi(cli_ftp, cmd_str[STORE], args);
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state == NEG_TMP)
		exit(EXIT_FAILURE_RETRY);
	else if (srv_verbs->sr_state == NEG_DEF)
		exit(EXIT_FAILURE);
	return 1;
}

int ftp_cli_cmd_print_workdir(t_cli_ftp *cli_ftp, const char *args)
{
	t_server_verbs *srv_verbs;
	char          **split;

	if (args)
	{
		put_no_req_arg(PRINT_WORKDIR);
		return 0;
	}
	ftp_cli_send_pi(cli_ftp, cmd_str[PRINT_WORKDIR], args);
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state == NEG_TMP)
		exit(EXIT_FAILURE_RETRY);
	else if (srv_verbs->sr_state == NEG_DEF)
		exit(EXIT_FAILURE);
	split = ft_strsplit(srv_verbs->user_info, '"');
	ft_printf("%s\n", split[0]);
	return 1;
}