/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 02:25:08 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 14:20:59 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_CLIENT_H
#define FTP_CLIENT_H

#include "ft_p.h"

typedef enum e_user_cmd
{
	LLS,
	LCD,
	LPWD,
	LS,
	CD,
	GET,
	PUT,
	PWD,
	QUIT,
	HELP,
} t_user_cmd;
typedef struct s_client_args
{
	struct in_addr ca_host;
	in_port_t      ca_port;
	char          *ca_user;
	char          *ca_pass;
	char          *ca_wdir;
} t_client_args;

typedef struct s_user_verbs
{
	t_user_cmd uv_code;
	char      *uv_arg;
} t_user_verbs;

typedef struct s_cli_transfert
{
	struct sockaddr_in sin;
	int                sock;
	SSL               *ssl;
	t_bool             ssl_activated;
} t_cli_transfert;

typedef struct s_cli_ftp
{
	t_cli_transfert pi;
	t_cli_transfert dtp;
	SSL_CTX       **ctx;
	char           *pwd;
} t_cli_ftp;

typedef int (*t_client_action)(t_cli_ftp *, const char *);

int ftp_cli_cmd_logout(t_cli_ftp *cli_ftp, const char *args);
int ftp_cli_cmd_auth(t_cli_ftp *cli_ftp);
int ftp_cli_cmd_password(t_cli_ftp *cli_ftp, t_client_args *args);
int ftp_cli_cmd_user(t_cli_ftp *cli_ftp, t_client_args *args);
int ftp_cli_cmd_change_workdir(t_cli_ftp *cli_ftp, const char *args);
int ftp_cli_cmd_print_workdir(t_cli_ftp *cli_ftp, const char *args);
int ftp_cli_cmd_help(t_cli_ftp *cli_ftp, const char *args);
int ftp_cli_cmd_protection_buffer_size(t_cli_ftp *cli_ftp);
int ftp_cli_cmd_protection(t_cli_ftp *cli_ftp);
int ftp_cli_cmd_passive_mode(t_cli_ftp *cli_ftp);
int ftp_cli_cmd_list(t_cli_ftp *cli_ftp, const char *args);
int ftp_cli_cmd_get_file(t_cli_ftp *cli_ftp, const char *args);
int ftp_cli_cmd_put_file(t_cli_ftp *cli_ftp, const char *args);
int ftp_cli_cmd_local_change_directory(t_cli_ftp *cli_ftp, const char *dirname);
int ftp_cli_cmd_local_print_directory(t_cli_ftp *cli_ftp, const char *args);
int ftp_cli_cmd_local_list(t_cli_ftp *cli_ftp, const char *args);

void put_req_arg(t_cmd cmd);
void put_no_req_arg(t_cmd cmd);

int ftp_cli_send_pi(t_cli_ftp *cli_ftp, const char *cmd, const char *args);
t_srv_res    *ftp_wait_for_response(t_cli_ftp *cli_ftp);
t_srv_res    *ftp_cli_srv_lexer(char *str);
t_user_verbs *ftp_cli_user_lexer(const char *str);

void ftp_cli_connection_protocol(t_cli_ftp *cli_ftp, t_client_args *ca);

#endif