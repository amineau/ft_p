/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_files_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 04:42:08 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 14:25:47 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

t_srv_res cmd_print_workdir(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;
	char     *wdir;
	// char           buff[BUFF_SIZE];

	(void)srv_ftp;
	if (req->req_arg)
		response = ftp_build_srv_res(_504, "Command without param only");
	else
	{
		wdir = get_wdir();
		response = ftp_build_srv_res(
			_257,
			ft_arrayjoin(
				(char *[]){"\"", wdir, "\" is the current directory", NULL}));
		ft_strdel(&wdir);
	}
	return response;
}

t_srv_res cmd_change_workdir(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;

	(void)srv_ftp;
	if (req->req_arg)
	{
		if (!ftp_change_wdir(req->req_arg))
			response = ftp_build_srv_res(_200, "Working directory changed");
		else
			response = ftp_build_srv_res(_431, "No such directory");
	}
	else
		response = ftp_build_srv_res(_504, "Command with param only");
	return response;
}

t_srv_res cmd_change_to_parent_dir(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;

	if (req->req_arg)
		response = ftp_build_srv_res(_504, "Command without param only");
	else
	{
		req->req_arg = ft_strdup("..");
		response = cmd_change_workdir(req, srv_ftp);
	}
	return response;
}

t_srv_res cmd_logout(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	(void)req;
	return (cmd_not_implemented(LOGOUT, srv_ftp));
}

t_srv_res cmd_representation_type(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;

	(void)srv_ftp;
	if (!ft_strncasecmp(req->req_arg, "I", 1))
		response = ftp_build_srv_res(_200, "");
	else
		response = ftp_build_srv_res(_504, "Not implemented");
	return (response);
}
t_srv_res cmd_retrieve(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;
	int       fd;
	char      buff[10];

	if ((fd = open(req->req_arg, O_RDONLY)) == -1)
		return (ftp_build_srv_res(_421, "Failed to open file"));
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
	response = ftp_build_srv_res(_226, "");
	close(fd);
	if (srv_ftp->pi.ssl_activated)
		shutdown_ssl(srv_ftp->dtp.ssl);
	close(srv_ftp->dtp.cs);
	return (response);
}
t_srv_res cmd_store(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	(void)req;
	return (cmd_not_implemented(STORE, srv_ftp));
}
t_srv_res cmd_rename_from(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	(void)req;
	return (cmd_not_implemented(RENAME_FROM, srv_ftp));
}
t_srv_res cmd_rename_to(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	(void)req;
	return (cmd_not_implemented(RENAME_TO, srv_ftp));
}
t_srv_res cmd_abort(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	(void)req;
	return (cmd_not_implemented(ABORT, srv_ftp));
}
t_srv_res cmd_delete(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	(void)req;
	return (cmd_not_implemented(DELETE, srv_ftp));
}
t_srv_res cmd_remove_dir(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	(void)req;
	return (cmd_not_implemented(REMOVE_DIR, srv_ftp));
}
t_srv_res cmd_make_dir(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	(void)req;
	return (cmd_not_implemented(MAKE_DIR, srv_ftp));
}

t_srv_res cmd_list(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;
	DIR      *dp;
	char     *cwd;
	char     *args;

	srv_ftp->dtp.cs = ftp_accept_connection(srv_ftp->dtp.sock, &srv_ftp->dtp.sin);
	if (srv_ftp->pi.ssl_activated)
	{
		srv_ftp->dtp.ssl = ftp_create_ssl(srv_ftp->dtp.cs, *srv_ftp->ctx);
		ftp_accept_ssl(srv_ftp->dtp.ssl);
	}
	cwd = ft_getcwd();
	if ((dp = opendir(cwd)) == NULL)
		return (ftp_build_srv_res(_421, "Failed to open working dir"));

	ftp_srv_send_pi(&srv_ftp->pi, _150, "");
	args = NULL;
	if (!ft_strcmp("-a", req->req_arg))
		args = "-a";
	ftp_srv_pipe_dtp(&srv_ftp->dtp,
					 "/bin/ls",
					 (char *const[]){"ls", "-nl", args, (char *)NULL});

	response = ftp_build_srv_res(_226, "");
	closedir(dp);
	ftp_close_connection(&srv_ftp->dtp);

	ft_strdel(&cwd);
	return (response);
}
t_srv_res cmd_system(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	(void)req;
	return (cmd_not_implemented(SYSTEM, srv_ftp));
}
t_srv_res cmd_noop(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	(void)req;
	return (cmd_not_implemented(NOOP, srv_ftp));
}