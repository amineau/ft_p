/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 20:07:43 by amineau           #+#    #+#             */
/*   Updated: 2022/04/22 21:35:52 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_P_H
#define FT_P_H

#include "libft.h"
#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <pthread.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_PENDING_CONNECTIONS 42
#define MAX_CLIENT_CONNECTION   8

#define TLS_VERSION "TLSv1.3"

#define CRLF "\r\n"
#define LF   '\n'

#define FTP_EOC CRLF
#define FTP_EOR 0xFF 0x01
#define FTP_EOF 0xFF 0x02

#define BUFF_SIZE_READ       1024
#define BUFF_SIZE_WRITE      1024
#define BUFF_SIZE_DATA_READ  1024
#define BUFF_SIZE_DATA_WRITE 24

#define EXIT_FAILURE_RETRY 2
#define EXIT_FAILURE       1
#define EXIT_SUCCESS       0

#define ANONYMOUS_USER "Anonymous"

/********** FTP COMMANDS **********/
#define USERNAME               "USER"
#define PASSWORD               "PASS"
#define ACCOUNT                "ACCT"
#define AUTH_METHOD            "AUTH"
#define CHANGE_WORKDIR         "CWD"
#define CHANGE_TO_PARENT_DIR   "CDUP"
#define LOGOUT                 "QUIT"
#define PORT                   "PORT"
#define PASSIVE_MODE           "PASV"
#define REPRESENTATION_TYPE    "TYPE"
#define RETRIEVE               "RETR"
#define STORE                  "STOR"
#define RENAME_FROM            "RNFR"
#define RENAME_TO              "RNTO"
#define ABORT                  "ABOR"
#define DELETE                 "DELE"
#define REMOVE_DIR             "RMD"
#define MAKE_DIR               "MKD"
#define PRINT_WORKDIR          "PWD"
#define LIST                   "LIST"
#define SYSTEM                 "SYST"
#define PROTECTION_BUFFER_SIZE "PBSZ"
#define PROTECTION             "PROT"
#define NOOP                   "NOOP"
/**********************************/

/*********** FTP CODE *************/
typedef enum e_ftp_code
{
	_100,
	_120,
	_125,
	_150,
	_200,
	_202,
	_211,
	_214,
	_220,
	_221,
	_226,
	_227,
	_230,
	_234,
	_250,
	_257,
	_331,
	_332,
	_350,
	_421,
	_425,
	_426,
	_430,
	_431,
	_450,
	_451,
	_452,
	_500,
	_501,
	_502,
	_503,
	_504,
	_506,
	_520,
	_530,
	_532,
	_536,
	_550,
	_551,
	_552,
	_553,
	_NOCODE
} t_ftp_code_enum;
/**********************************/

// static pthread_mutex_t	g_mutex_stock = PTHREAD_MUTEX_INITIALIZER;

typedef enum e_bool
{
	false,
	true
} t_bool;

typedef enum e_state
{
	POS_TMP = 1,
	POS_DEF,
	POS_INT,
	NEG_TMP,
	NEG_DEF
} t_state;

typedef struct s_client_args
{
	struct in_addr ca_host;
	in_port_t      ca_port;
	char          *ca_user;
	char          *ca_pass;
	char          *ca_wdir;
} t_client_args;

typedef struct s_server_args
{
	int    sa_port;
	char  *sa_root;
	t_bool sa_debug;
} t_server_args;

typedef struct s_client_verbs
{
	char		 *cv_verb;
	char		 *cv_arg;
	int           cv_code;
	pam_handle_t *cv_pamh;
} t_client_verbs;

typedef struct s_server_verbs
{
	t_ftp_code_enum sr_code;
	t_state         sr_state;
	char           *user_info;
} t_server_verbs;

typedef struct s_srv_transfert
{
	int    cs;
	int    sock;
	SSL   *ssl;
	t_bool ssl_activated;
} t_srv_transfert;

typedef struct s_srv_ftp
{
	t_srv_transfert pi;
	t_srv_transfert dtp;
	SSL_CTX       **ctx;
} t_srv_ftp;

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
} t_cli_ftp;

typedef int (*t_client_action)(t_cli_ftp *, const char *);
typedef t_server_verbs (*t_server_action)(t_client_verbs *, t_srv_ftp *);

extern t_bool debug;

void error_print_exit(int status, const char *err);

char *ft_getcwd(void);
char *get_root(void);
void  init_root_static(void);
char *get_wdir(void);
int   ftp_change_wdir(const char *dir);

t_server_verbs cmd_not_implemented(char *str, t_srv_ftp *srv_ftp);
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

int ftp_cli_cmd_logout(t_cli_ftp *cli_ftp, const char *args);
int ftp_cli_cmd_auth(t_cli_ftp *cli_ftp);
int ftp_cli_cmd_password(t_cli_ftp *cli_ftp, t_client_args *args);
int ftp_cli_cmd_user(t_cli_ftp *cli_ftp, t_client_args *args);
int ftp_cli_cmd_change_workdir(t_cli_ftp *cli_ftp, const char *args);
int ftp_client_cmd_print_workdir(t_cli_ftp *cli_ftp, const char *args);
int ftp_cli_cmd_help(t_cli_ftp *cli_ftp, const char *args);
int ftp_cli_cmd_protection_buffer_size(t_cli_ftp *cli_ftp);
int ftp_cli_cmd_protection(t_cli_ftp *cli_ftp);
int ftp_cli_cmd_passive_mode(t_cli_ftp *cli_ftp);
int ftp_cli_cmd_list(t_cli_ftp *cli_ftp, const char *args);

void put_req_arg(char *cmd);
void put_no_req_arg(char *cmd);

int             ftp_cli_send_pi(t_cli_ftp *cli_ftp, const char *cmd, const char *args);
t_server_verbs *ftp_wait_for_response(t_cli_ftp *cli_ftp);
t_server_verbs *ftp_cli_srv_lexer(char *str);
int             ftp_cli_user_lexer(const char *buff, t_client_verbs *cv);
void            ftp_cli_connection_protocol(t_cli_ftp *cli_ftp, t_client_args *ca);

struct sockaddr_in ftp_get_socket_address(struct in_addr addr, in_port_t port);
void               ftp_connect_socket(int sock, struct sockaddr_in *sin);
int                ftp_create_socket();

int  ftp_create_channel(int sock);
int  ftp_accept_connection(int sock);
int  ftp_srv_send_pi(t_srv_transfert *srv_tranfert, t_ftp_code_enum code, char *description);
int  ftp_srv_send_dtp(t_srv_transfert *srv_tranfert, char *data);
void ftp_srv_pipe_dtp(t_srv_transfert *srv_tranfert, const char *path, char *const argv[]);

// int get_next_line_wrapper(t_srv_transfert *srv_transfert, char *buff);
int get_next_line_wrapper(int fd, SSL *ssl, t_bool ssl_activate, char **buff);

void     init_openssl();
void     cleanup_openssl();
void     shutdown_ssl(SSL *ssl);
SSL_CTX *ftp_srv_create_context();
SSL_CTX *ftp_cli_create_context();
void     configure_context(SSL_CTX *ctx);
void     ShowCerts(SSL *ssl);
SSL     *ftp_create_ssl(int sock, SSL_CTX *ctx);
void     ftp_connect_ssl(SSL *ssl);

t_state         ftp_get_state_code(char *code);
int             ftp_is_valid_response_code(char *code);
t_ftp_code_enum ftp_get_ftp_code_enum(const char *code);
struct in_addr  htoaddr(char *name);
struct in_addr  stoaddr(in_addr_t s_addr);

#endif
