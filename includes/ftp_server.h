/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 02:25:08 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 14:31:29 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_SERVER_H
#define FTP_SERVER_H

#include "ft_p.h"
#include <security/pam_appl.h>

typedef struct s_server_args
{
	in_port_t sa_port;
	char     *sa_root;
	t_bool    sa_debug;
} t_server_args;

typedef struct s_cli_req
{
	t_cmd req_code;
	char *req_arg;
} t_cli_req;

typedef struct s_srv_transfert
{
	struct sockaddr_in sin;
	int                cs;
	int                sock;
	SSL               *ssl;
	t_bool             ssl_activated;
} t_srv_transfert;

typedef struct s_srv_ftp
{
	t_srv_transfert pi;
	t_srv_transfert dtp;
	SSL_CTX       **ctx;
	pam_handle_t   *pamh;
} t_srv_ftp;

typedef t_srv_res (*t_server_action)(t_cli_req *, t_srv_ftp *);

char *get_root(void);
void  init_root_static(void);
char *get_wdir(void);
int   ftp_change_wdir(const char *dir);

void ftp_close_connection(t_srv_transfert *srv_transfert);

t_srv_res cmd_not_implemented(t_cmd cmd, t_srv_ftp *srv_ftp);
t_srv_res cmd_username(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_password(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_account(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_auth_method(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_change_workdir(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_change_to_parent_dir(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_logout(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_port(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_passive_mode(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_representation_type(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_retrieve(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_store(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_rename_from(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_rename_to(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_abort(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_delete(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_remove_dir(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_make_dir(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_print_workdir(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_list(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_system(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_protection_buffer_size(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_protection(t_cli_req *req, t_srv_ftp *srv_ftp);
t_srv_res cmd_noop(t_cli_req *req, t_srv_ftp *srv_ftp);

int  ftp_srv_send_pi(t_srv_transfert *srv_tranfert,
					 t_ftp_code_enum  code,
					 char            *description);
int  ftp_srv_send_dtp(t_srv_transfert *srv_tranfert, char *data);
void ftp_srv_pipe_dtp(t_srv_transfert *srv_tranfert,
					  const char      *path,
					  char *const      argv[]);

#endif