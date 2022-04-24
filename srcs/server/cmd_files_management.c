/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_files_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 04:42:08 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 02:42:16 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

t_server_verbs cmd_print_workdir(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_verbs sv;
	char          *wdir;
	// char           buff[BUFF_SIZE];

	(void)srv_ftp;
	if (cv->cv_arg)
	{
		sv.sr_code = _504;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Command without param only";
	}
	else
	{
		wdir = get_wdir();
		// sft_printf(buff, "\"%s\" is the current directory", wdir);
		sv.sr_code = _257;
		sv.sr_state = POS_DEF;
		sv.user_info = ft_arrayjoin(
			(char *[]){"\"", wdir, "\" is the current directory", NULL});
		ft_strdel(&wdir);
	}
	return sv;
}

t_server_verbs cmd_change_workdir(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_verbs sv;

	(void)srv_ftp;
	if (cv->cv_arg)
	{
		if (!ftp_change_wdir(cv->cv_arg))
		{
			sv.sr_code = _200;
			sv.sr_state = POS_DEF;
			sv.user_info = "Working directory changed";
		}
		else
		{
			sv.sr_code = _431;
			sv.sr_state = NEG_TMP;
			sv.user_info = "No such directory";
		}
	}
	else
	{
		sv.sr_code = _504;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Command with param only";
	}
	return sv;
}

t_server_verbs cmd_change_to_parent_dir(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_verbs sv;

	if (cv->cv_arg)
	{
		sv.sr_code = _504;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Command without param only";
		return sv;
	}
	cv->cv_arg = ft_strdup("..");
	return cmd_change_workdir(cv, srv_ftp);
}

t_server_verbs cmd_logout(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	(void)cv;
	return (cmd_not_implemented(LOGOUT, srv_ftp));
}

t_server_verbs cmd_representation_type(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_verbs sv;

	(void)srv_ftp;
	if (!ft_strncasecmp(cv->cv_arg, "I", 1))
	{
		sv.sr_code = _200;
		sv.sr_state = POS_DEF;
		sv.user_info = "";
	}
	else
	{
		sv.sr_code = _504;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Not implemented";
	}
	return (sv);
}
t_server_verbs cmd_retrieve(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_verbs sv;
	int            fd;
	char           buff[10];

	if ((fd = open(cv->cv_arg, O_RDONLY)) == -1)
	{
		sv.sr_code = _421;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Failed to open file";
		return sv;
	}
	srv_ftp->dtp.cs = ftp_accept_connection(srv_ftp->dtp.sock, &srv_ftp->dtp.sin);
	if (srv_ftp->pi.ssl_activated)
	{
		srv_ftp->dtp.ssl = ftp_create_ssl(srv_ftp->dtp.cs, *srv_ftp->ctx);
		ftp_accept_ssl(srv_ftp->dtp.ssl);
	}

	ftp_srv_send_pi(&srv_ftp->pi, _150, "");
	while (read(fd, buff, 10 - 1) > 0)
	{
		buff[10 - 1] = '\0';
		ftp_srv_send_dtp(&srv_ftp->dtp, buff);
		ft_bzero(buff, 10);
	}
	sv.sr_code = _226;
	sv.sr_state = POS_DEF;
	sv.user_info = "";
	close(fd);
	if (srv_ftp->pi.ssl_activated)
		shutdown_ssl(srv_ftp->dtp.ssl);
	close(srv_ftp->dtp.cs);
	return (sv);
}
t_server_verbs cmd_store(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	(void)cv;
	return (cmd_not_implemented(STORE, srv_ftp));
}
t_server_verbs cmd_rename_from(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	(void)cv;
	return (cmd_not_implemented(RENAME_FROM, srv_ftp));
}
t_server_verbs cmd_rename_to(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	(void)cv;
	return (cmd_not_implemented(RENAME_TO, srv_ftp));
}
t_server_verbs cmd_abort(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	(void)cv;
	return (cmd_not_implemented(ABORT, srv_ftp));
}
t_server_verbs cmd_delete(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	(void)cv;
	return (cmd_not_implemented(DELETE, srv_ftp));
}
t_server_verbs cmd_remove_dir(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	(void)cv;
	return (cmd_not_implemented(REMOVE_DIR, srv_ftp));
}
t_server_verbs cmd_make_dir(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	(void)cv;
	return (cmd_not_implemented(MAKE_DIR, srv_ftp));
}

t_server_verbs cmd_list(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_verbs sv;
	DIR           *dp;
	char          *cwd;
	char          *args;

	srv_ftp->dtp.cs = ftp_accept_connection(srv_ftp->dtp.sock, &srv_ftp->dtp.sin);
	if (srv_ftp->pi.ssl_activated)
	{
		srv_ftp->dtp.ssl = ftp_create_ssl(srv_ftp->dtp.cs, *srv_ftp->ctx);
		ftp_accept_ssl(srv_ftp->dtp.ssl);
	}
	cwd = ft_getcwd();
	if ((dp = opendir(cwd)) == NULL)
	{
		sv.sr_code = _421;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Failed to open working dir";
		return sv;
	}

	ftp_srv_send_pi(&srv_ftp->pi, _150, "");
	args = NULL;
	if (!ft_strcmp("-a", cv->cv_arg))
		args = "-a";
	ftp_srv_pipe_dtp(&srv_ftp->dtp,
					 "/bin/ls",
					 (char *const[]){"ls", "-nl", args, (char *)NULL});

	sv.sr_code = _226;
	sv.sr_state = POS_DEF;
	sv.user_info = "";
	closedir(dp);
	if (srv_ftp->pi.ssl_activated)
		shutdown_ssl(srv_ftp->dtp.ssl);
	close(srv_ftp->dtp.cs);

	ft_strdel(&cwd);
	return (sv);
}
t_server_verbs cmd_system(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	(void)cv;
	return (cmd_not_implemented(SYSTEM, srv_ftp));
}
t_server_verbs cmd_noop(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	(void)cv;
	return (cmd_not_implemented(NOOP, srv_ftp));
}