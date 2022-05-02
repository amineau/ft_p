/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 20:07:43 by amineau           #+#    #+#             */
/*   Updated: 2022/05/02 20:06:58 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_P_H
#define FT_P_H

#include "libft.h"
#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <netdb.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_PENDING_CONNECTIONS 2
#define MAX_CLIENT_CONNECTION   2

#define TLS_VERSION "TLSv1.3"

#define CRLF "\r\n"
#define LF   "\n"

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

/*********** FTP CODE *************/

typedef enum e_cmd
{
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
	PROTECTION_BUFFER_SIZE,
	PROTECTION,
	MODE,
	STRUCTURE,
	NOOP,
} t_cmd;

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

typedef struct s_srv_res
{
	t_ftp_code_enum sr_code;
	t_state         sr_state;
	char           *user_info;
} t_srv_res;

extern t_bool debug;

void error_print_exit(int status, const char *err);

char *ft_getcwd(void);

struct sockaddr_in ftp_get_socket_address(struct in_addr addr, in_port_t port);
void               ftp_connect_socket(int sock, struct sockaddr_in *sin);
int                ftp_create_socket();
int                ftp_accept_connection(int sock, struct sockaddr_in *sin);
void               ftp_bind_socket(int sock, struct sockaddr_in *sin);
int                ftp_listen_connection(int sock);

int get_next_line_wrapper(
	int fd, SSL *ssl, t_bool ssl_activate, char **buff, char *eol);

void     init_openssl();
void     cleanup_openssl();
void     shutdown_ssl(SSL *ssl);
SSL_CTX *ftp_srv_create_context();
SSL_CTX *ftp_cli_create_context();
void     configure_context(SSL_CTX *ctx);
void     ShowCerts(SSL *ssl);
SSL     *ftp_create_ssl(int sock, SSL_CTX *ctx);
void     ftp_connect_ssl(SSL *ssl);
void     ftp_accept_ssl(SSL *ssl);

t_state         ftp_get_state_code(t_ftp_code_enum code);
int             ftp_is_valid_response_code(char *code);
t_ftp_code_enum ftp_get_code_enum(const char *code);
struct in_addr  htoaddr(char *name);
struct in_addr  stoaddr(in_addr_t s_addr);

in_addr_t itoaddr(int *numbers);
in_port_t itoport(int *numbers);
char	 *porttostr(in_port_t sin_port);
char	 *get_local_ip(char *interface);
t_bool    is_interface_online(char *interface);

// move to ftp_server.h
t_srv_res ftp_build_srv_res(t_ftp_code_enum code, char *user_info);

#endif
