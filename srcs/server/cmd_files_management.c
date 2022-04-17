/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_files_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 04:42:08 by amineau           #+#    #+#             */
/*   Updated: 2022/04/17 23:21:29 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_server_verbs	cmd_print_workdir(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	t_server_verbs	sv;
	char*			wdir;
	char			buff[BUFF_SIZE];

	if (cv->cv_arg)
	{
		sv.sr_code = _504;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Command without param only";
	}
	else
	{
		wdir = get_wdir();
		sprintf(buff, "\"%s\" is the current directory", wdir);
		ft_strdel(&wdir);
		sv.sr_code = _257;
		sv.sr_state = POS_DEF;
		sv.user_info = buff;
	}
	return (sv);
}

t_server_verbs	cmd_change_workdir(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(CHANGE_WORKDIR, srv_ftp));
} 
t_server_verbs	cmd_change_to_parent_dir(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(CHANGE_TO_PARENT_DIR, srv_ftp));
} 
t_server_verbs	cmd_logout(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(LOGOUT, srv_ftp));
}

t_server_verbs	cmd_representation_type(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	t_server_verbs	sv;
	char*			wdir;
	char			buff[BUFF_SIZE];

	if (!ft_strcasecmp(cv->cv_arg, "I")) {
		sv.sr_code = _200;
		sv.sr_state = POS_DEF;
		sv.user_info = "";
		return (sv);
	}
	return cmd_not_implemented(REPRESENTATION_TYPE, srv_ftp);
}
t_server_verbs	cmd_retrieve(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(RETRIEVE, srv_ftp));
} 
t_server_verbs	cmd_store(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(STORE, srv_ftp));
} 
t_server_verbs	cmd_rename_from(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(RENAME_FROM, srv_ftp));
} 
t_server_verbs	cmd_rename_to(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(RENAME_TO, srv_ftp));
} 
t_server_verbs	cmd_abort(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(ABORT, srv_ftp));
} 
t_server_verbs	cmd_delete(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(DELETE, srv_ftp));
} 
t_server_verbs	cmd_remove_dir(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(REMOVE_DIR, srv_ftp));
} 
t_server_verbs	cmd_make_dir(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(MAKE_DIR, srv_ftp));
} 
t_server_verbs	cmd_list(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	t_server_verbs	sv;
	struct dirent*	dir;
	DIR*			dp;
	char			buff[BUFF_SIZE];
	int				i;
	int				j;
	char*			cwd;
	char*			wdir;

	(void)cv;

	srv_ftp->dtp.cs = ftp_accept_connection(srv_ftp->dtp.sock);
	srv_ftp->dtp.ssl= SSL_new(*srv_ftp->ctx);
	SSL_set_fd(srv_ftp->dtp.ssl, srv_ftp->dtp.cs);
	
	if (SSL_accept(srv_ftp->dtp.ssl) <= 0)
	{
		printf("Warning: SSL failed");
		ERR_print_errors_fp(stderr);
		shutdown_ssl(srv_ftp->dtp.ssl);
	}

	cwd = ft_getcwd();
	wdir = get_wdir();
	if ((dp = opendir(cwd)) == NULL)
	{
		sv.sr_code = _421;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Failed to open working dir";
		return sv;
	}

	ftp_srv_send_pi(srv_ftp, _150, "");
	// i = 1;
	// errno = 0;
	// buff[0] = '\n';
	while((dir = readdir(dp)) != NULL)
	{
		ftp_srv_send_dtp(srv_ftp, dir->d_name, ft_strlen(dir->d_name));
		// j = -1;
		// while(wdir[++j])
		// 	buff[i +  j] = wdir[j];
		// i += j;
		// j = -1;		
		// while(dir->d_name[++j])
		// 	buff[i + j] = dir->d_name[j];
		// i += j + 1;
		// buff[i - 1] = ' ';
	}
	// buff[i - 1] = '\0';
	// printf("buff : [%s]\n", buff);
	sv.sr_code = _226;
	sv.sr_state = POS_DEF;
	sv.user_info = "";
	closedir(dp);
	// ftp_srv_send_pi(srv_ftp, _NOCODE, buff);
	// printf("coucou\n");

	ft_strdel(&cwd);
    return (sv);
} 
t_server_verbs	cmd_system(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(SYSTEM, srv_ftp));
} 
t_server_verbs	cmd_noop(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
	(void)cv;
    return (cmd_not_implemented(NOOP, srv_ftp));
}