/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responses.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 23:50:22 by amineau           #+#    #+#             */
/*   Updated: 2022/04/22 01:39:04 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_server_verbs *listen_server(t_cli_ftp *cli_ftp)
{
	int             ret;
	char           *buff;
	t_server_verbs *srv_verbs;

	srv_verbs = NULL;
	while (!srv_verbs &&
		   (ret = get_next_line_wrapper(
				cli_ftp->pi.sock,
				cli_ftp->pi.ssl,
				cli_ftp->pi.ssl_activated,
				&buff)) > 0)
	{
		if (debug)
			printf("\033[0;32mSERVER-PI: %s\033[0m\n", buff);
		srv_verbs = ftp_cli_srv_lexer(buff);
	}

	if (ret == -1)
		exit(EXIT_FAILURE_RETRY);
	return (srv_verbs);
}

t_server_verbs *ftp_wait_for_response(t_cli_ftp *cli_ftp)
{
	t_server_verbs *srv_verbs;

	if ((srv_verbs = listen_server(cli_ftp)))
		return srv_verbs;
	exit(EXIT_FAILURE_RETRY);
}

int ftp_cli_send_pi(t_cli_ftp *cli_ftp, const char *cmd, const char *args)
{
	int   ret;
	char *str;

	if (debug == true)
	{
		if (!ft_strcmp(cmd, PASSWORD))
			ft_printf("\033[0;34mUSER-PI: %s ****\n\033[0m", cmd);
		else if (args)
			ft_printf("\033[0;34mUSER-PI: %s %s\n\033[0m", cmd, args);
		else
			ft_printf("\033[0;34mUSER-PI: %s\n\033[0m", cmd);
	}
	if (args)
		str = ft_strcln2join(ft_straddc(cmd, ' '), ft_strjoin(args, FTP_EOC));
	else
		str = ft_strjoin(cmd, FTP_EOC);
	if (cli_ftp->pi.ssl_activated == true)
		ret = SSL_write(cli_ftp->pi.ssl, str, ft_strlen(str));
	else
		ret = write(cli_ftp->pi.sock, str, ft_strlen(str));
	free(str);
	return (ret);
}
