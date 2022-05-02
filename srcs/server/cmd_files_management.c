/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_files_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 04:42:08 by amineau           #+#    #+#             */
/*   Updated: 2022/05/02 20:21:12 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

void ftp_connect_dtp(t_srv_ftp *srv_ftp)
{
	if (srv_ftp->conf.passive == 1)
		srv_ftp->dtp.cs =
			ftp_accept_connection(srv_ftp->dtp.sock, &srv_ftp->dtp.sin);
	else
	{
		srv_ftp->dtp.cs = ftp_create_socket();
		ftp_connect_socket(srv_ftp->dtp.cs, &srv_ftp->dtp.sin);
	}
	if (srv_ftp->dtp.ssl_activated)
	{
		srv_ftp->dtp.ssl = ftp_create_ssl(srv_ftp->dtp.cs, *srv_ftp->ctx);
		ftp_accept_ssl(srv_ftp->dtp.ssl);
	}
}

t_srv_res cmd_print_workdir(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;
	char     *wdir;

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
		if (ftp_change_wdir(req->req_arg) == -1)
			response = ftp_build_srv_res(_431, "No such directory");
		else
			response = ftp_build_srv_res(_200, "Working directory changed");
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

char *next_word(char *str)
{
	while (*str && *str != ' ')
		str++;
	return str;
}

int ft_indexof(const char *str, char c)
{
	int i;

	i = 0;
	while (str && str[i] != c)
		i++;
	if (!str)
		i = -1;
	return (i);
}

t_srv_res cmd_representation_type(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	int        ret;
	char      *format_arg;
	const char types[5] = {
		[ASCII] = 'A', [EBCDIC] = 'E', [IMAGE] = 'I', [LOCAL] = 'L', 0};
	const char formats[4] = {
		[NON_PRINT] = 'N', [TELNET] = 'T', [CARRIAGE_CONTROL] = 'C', 0};

	ret = ft_indexof(types, ft_toupper(*req->req_arg));
	if (ret == -1)
		return (ftp_build_srv_res(_501, "Syntax error in the arguments"));
	srv_ftp->conf.repr.type = (t_repr)ret;
	if (srv_ftp->conf.repr.type == ASCII || srv_ftp->conf.repr.type == EBCDIC)
	{
		if ((format_arg = next_word(req->req_arg)))
		{
			ret = ft_indexof(formats, ft_toupper(*format_arg));
			if (ret == -1)
				return (ftp_build_srv_res(_501, "Syntax error in the arguments"));
			srv_ftp->conf.repr.format = (t_data_format)ret;
		}
		else
			srv_ftp->conf.repr.format = NON_PRINT;
	}
	else if (srv_ftp->conf.repr.type == LOCAL)
		return (ftp_build_srv_res(_503, "Not implemented"));
	return (ftp_build_srv_res(_200, ""));
}

t_srv_res cmd_mode(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	const char modes[4] = {[STREAM] = 'S', [BLOCK] = 'B', [COMPRESSED] = 'C', 0};
	t_srv_res  response;
	int        ret;

	ret = ft_indexof(modes, ft_toupper(*req->req_arg));
	if (ret == -1)
		return (ftp_build_srv_res(_501, "Syntax error in the arguments"));
	srv_ftp->conf.mode = (t_transfert_mode)ret;
	if (srv_ftp->conf.mode == BLOCK || srv_ftp->conf.mode == COMPRESSED)
		response = ftp_build_srv_res(_503, "Not implemented");
	else
		response = ftp_build_srv_res(_200, "");
	return (response);
}

t_srv_res cmd_file_structure(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	const char structures[4] = {
		[FILE_STRUCT] = 'F', [RECORD_STRUCT] = 'R', [PAGE_STRUCT] = 'P', 0};
	t_srv_res response;
	int       ret;

	ret = ft_indexof(structures, ft_toupper(*req->req_arg));
	if (ret == -1)
		return (ftp_build_srv_res(_501, "Syntax error in the arguments"));
	srv_ftp->conf.file_struct = (t_file_struct)ret;
	if (srv_ftp->conf.file_struct == RECORD_STRUCT ||
		srv_ftp->conf.file_struct == PAGE_STRUCT)
		response = ftp_build_srv_res(_503, "Not implemented");
	else
		response = ftp_build_srv_res(_200, "");
	return (response);
}

t_srv_res cmd_retrieve(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;
	int       fd;
	char      buff[BUFF_SIZE];

	if ((fd = ftp_open_file(req->req_arg, O_RDONLY)) == -1)
		return (ftp_build_srv_res(_421, "Failed to open file"));

	ftp_connect_dtp(srv_ftp);
	ftp_srv_response_pi(&srv_ftp->pi, _150, "");
	while (read(fd, buff, BUFF_SIZE - 1) > 0)
	{
		buff[BUFF_SIZE - 1] = '\0';
		ftp_srv_response_dtp(&srv_ftp->dtp, buff);
		ft_bzero(buff, BUFF_SIZE);
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

	ftp_connect_dtp(srv_ftp);
	cwd = ft_getcwd();
	if ((dp = opendir(cwd)) == NULL)
		return (ftp_build_srv_res(_421, "Failed to open working dir"));

	ftp_srv_response_pi(&srv_ftp->pi, _150, "");
	args = NULL;
	if (req->req_arg && !ft_strcmp("-a", req->req_arg))
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
	(void)srv_ftp;
	return (ftp_build_srv_res(_200, "Ok"));
}