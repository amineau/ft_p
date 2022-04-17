/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 20:07:43 by amineau           #+#    #+#             */
/*   Updated: 2022/04/18 01:24:43 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_P_H
# define FT_P_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>
# include <pthread.h>
# include <dirent.h>
# include <signal.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/wait.h>
# include <security/pam_appl.h>
# include <security/pam_misc.h>
# include <openssl/ssl.h>
# include <openssl/crypto.h>
# include <openssl/err.h>

// Debug
# include <string.h>
// End Debug
# include "libft.h"

# define MAX_PENDING_CONNECTIONS 42
# define MAX_CLIENT_CONNECTION 8

# define TLS_VERSION "TLSv1.3"

# define CRLF "\r\n"
# define LF '\n'

# define FTP_EOC CRLF

/********** FTP COMMANDS **********/
# define USERNAME "USER"
# define PASSWORD "PASS"
# define ACCOUNT  "ACCT"
# define AUTH_METHOD "AUTH"
# define CHANGE_WORKDIR "CWD"
# define CHANGE_TO_PARENT_DIR "CDUP"
# define LOGOUT "QUIT"
# define PORT "PORT"
# define PASSIVE_MODE "PASV"
# define REPRESENTATION_TYPE "TYPE"
# define RETRIEVE "RETR"
# define STORE "STOR" 
# define RENAME_FROM "RNFR"
# define RENAME_TO "RNTO"
# define ABORT "ABOR"
# define DELETE "DELE"
# define REMOVE_DIR "RMD"
# define MAKE_DIR "MKD"
# define PRINT_WORKDIR "PWD"
# define LIST "LIST"
# define SYSTEM "SYST"
# define NOOP "NOOP"
/**********************************/

/*********** FTP CODE *************/
typedef enum	e_ftp_code
{
	_100, _120, _125, _150,
	_200, _202, _211, _214, _220, _221, _226, _227,_230, _234, _250, _257,
	_331, _332, _350,
	_421, _425, _426, _430, _450, _451, _452,
	_500, _501, _502, _503, _504, _506, _520, _530, _532, _550, _551, _552, _553,
	_NOCODE
}				t_ftp_code_enum;

static const char *g_ftp_code_str[] = {
	"100", "120", "125", "150",
	"200", "202", "211", "214", "220", "221", "226", "227", "230", "234", "250", "257",
	"331", "332", "350",
	"421", "425", "426", "430", "450", "451", "452",
	"500", "501", "502", "503", "504", "506", "520", "530", "532",
	"550", "551", "552", "553",
	"",
	NULL
};
/**********************************/


// static pthread_mutex_t	g_mutex_stock = PTHREAD_MUTEX_INITIALIZER;

static const char*	g_ftp_cmd_str[] = {
	USERNAME,
	PASSWORD,
	ACCOUNT,
	AUTH_METHOD,
	CHANGE_WORKDIR,
	CHANGE_TO_PARENT_DIR,
	LOGOUT,
	PORT,
	PASSIVE_MODE,
	REPRESENTATION_TYPE,
	RETRIEVE,
	STORE,
	RENAME_FROM,
	RENAME_TO,
	ABORT,
	DELETE,
	REMOVE_DIR,
	MAKE_DIR,
	PRINT_WORKDIR,
	LIST,
	SYSTEM,
	NOOP,
	NULL
};

static const char*	g_user_cmd_str[] = {
	"ls",
	"cd",
	"get",
	"put",
	"pwd",
	"quit",
	"help",
	NULL
};

typedef enum	e_bool
{
	false,
	true
}				t_bool;

typedef enum	e_state
{
	POS_TMP,
	POS_DEF,
	POS_INT,
	NEG_TMP,
	NEG_DEF
}				t_state;

typedef struct	s_client_args
{
	struct in_addr	ca_host;
	int				ca_port;
	char*			ca_user;
	char*			ca_pass;
	char*			ca_wdir;
	t_bool			debug;
}				t_client_args;

typedef struct	s_server_args
{
	int		sa_port;
	char*	sa_root;
	t_bool	debug;
}				t_server_args;

typedef struct	s_client_verbs
{
	char*			cv_verb;
	char*			cv_arg;
	int				cv_code;
	pam_handle_t*	cv_pamh;
}				t_client_verbs;

typedef struct	s_server_verbs
{
	t_ftp_code_enum	sr_code;
	t_state			sr_state;
	char*			user_info;
}				t_server_verbs;

typedef struct s_srv_transfert
{
	int cs;
	int sock;
	SSL	*ssl;
}				t_srv_transfert;

typedef struct	s_svr_ftp
{
	t_srv_transfert	pi;
	t_srv_transfert	dtp;
	SSL_CTX **ctx;
	t_bool	ssl_activated;
	t_bool	debug;
}				t_srv_ftp;

typedef char			*(*t_client_action)(t_client_verbs*, int sock);
typedef t_server_verbs	(*t_server_action)(t_client_verbs*, t_srv_ftp*);


char*			ft_getcwd(void);
char*			get_root(void);
char*			get_wdir(void);

t_server_verbs  cmd_not_implemented(char* str, t_srv_ftp* srv_ftp);
t_server_verbs  cmd_username(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs  cmd_password(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs  cmd_account(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs  cmd_auth_method(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_change_workdir(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_change_to_parent_dir(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_logout(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_port(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_passive_mode(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_representation_type(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_retrieve(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_store(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_rename_from(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_rename_to(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_abort(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_delete(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_remove_dir(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_make_dir(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_print_workdir(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_list(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_system(t_client_verbs *cv, t_srv_ftp* srv_ftp);
t_server_verbs	cmd_noop(t_client_verbs *cv, t_srv_ftp* srv_ftp);

int				ftp_create_channel(int sock);
int				ftp_accept_connection(int sock);
int				ftp_srv_send_pi(t_srv_ftp* srv_ftp, t_ftp_code_enum code, char *description);
int				ftp_srv_send_dtp(t_srv_ftp* srv_ftp, char *data, int len);
int				received(t_srv_ftp* srv_ftp, char* buff);

void			init_openssl();
void			cleanup_openssl();
void			shutdown_ssl(SSL *ssl);
SSL_CTX			*ftp_srv_create_context();
SSL_CTX			*ftp_client_create_context();
void			configure_context(SSL_CTX *ctx);
void			ShowCerts(SSL* ssl);


#endif
