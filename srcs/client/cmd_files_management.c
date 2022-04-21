/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_files_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 23:43:53 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 01:34:56 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

// int		ftp_cli_cmd_list(t_cli_ftp *cli_ftp)
// {
// 	return (ft_strcjoin(LIST, cv->cv_arg, ' '));
// }

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