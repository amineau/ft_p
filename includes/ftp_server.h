/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 02:25:08 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 02:41:31 by amineau          ###   ########.fr       */
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
	pam_handle_t   *cv_pamh;
} t_srv_ftp;

typedef t_server_verbs (*t_server_action)(t_client_verbs *, t_srv_ftp *);

char *get_root(void);
void  init_root_static(void);
char *get_wdir(void);
int   ftp_change_wdir(const char *dir);

t_server_verbs cmd_not_implemented(t_cmd cmd, t_srv_ftp *srv_ftp);
t_server_verbs cmd_username(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_password(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_account(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_auth_method(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_change_workdir(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_change_to_parent_dir(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_logout(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_port(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_passive_mode(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_representation_type(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_retrieve(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_store(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_rename_from(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_rename_to(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_abort(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_delete(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_remove_dir(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_make_dir(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_print_workdir(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_list(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_system(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_protection_buffer_size(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_protection(t_client_verbs *cv, t_srv_ftp *srv_ftp);
t_server_verbs cmd_noop(t_client_verbs *cv, t_srv_ftp *srv_ftp);

int  ftp_srv_send_pi(t_srv_transfert *srv_tranfert,
					 t_ftp_code_enum  code,
					 char            *description);
int  ftp_srv_send_dtp(t_srv_transfert *srv_tranfert, char *data);
void ftp_srv_pipe_dtp(t_srv_transfert *srv_tranfert,
					  const char      *path,
					  char *const      argv[]);

#endif