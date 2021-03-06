/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 20:07:43 by amineau           #+#    #+#             */
/*   Updated: 2018/08/18 03:20:47 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_P_H
# define FT_P_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>
# include <pthread.h>
# include <security/pam_appl.h>
# include <security/pam_misc.h>
# include <openssl/ssl.h>
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
	_200, _202, _211, _214, _220, _221, _226, _230, _250, _257,
	_331, _332, _350,
	_421, _425, _426, _430, _450, _451, _452,
	_500, _501, _502, _503, _504, _506, _520, _530, _532, _550, _551, _552, _553
}				t_ftp_code_enum;

static const char *g_ftp_code_str[] = {
	"100", "120", "125", "150",
	"200", "202", "211", "214", "220", "221", "226", "230", "250", "257",
	"331", "332", "350",
	"421", "425", "426", "430", "450", "451", "452",
	"500", "501", "502", "503", "504", "506", "520", "530", "532",
	"550", "551", "552", "553",
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
}				t_client_args;

typedef struct	s_server_args
{
	int		sa_port;
	char*	sa_root;
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


typedef char			*(*t_client_action)(t_client_verbs*);
typedef t_server_verbs	(*t_server_action)(t_client_verbs*);


char*			ft_getcwd(void);
char*			get_root(void);
char*			get_wdir(void);

t_server_verbs  cmd_not_implemented(void);
t_server_verbs  cmd_username(t_client_verbs *cv);
t_server_verbs  cmd_password(t_client_verbs *cv);
t_server_verbs  cmd_account(t_client_verbs *cv);
t_server_verbs  cmd_auth_method(t_client_verbs *cv);
t_server_verbs	cmd_change_workdir(t_client_verbs *cv);
t_server_verbs	cmd_change_to_parent_dir(t_client_verbs *cv);
t_server_verbs	cmd_logout(t_client_verbs *cv);
t_server_verbs	cmd_representation_type(t_client_verbs *cv);
t_server_verbs	cmd_retrieve(t_client_verbs *cv);
t_server_verbs	cmd_store(t_client_verbs *cv);
t_server_verbs	cmd_rename_from(t_client_verbs *cv);
t_server_verbs	cmd_rename_to(t_client_verbs *cv);
t_server_verbs	cmd_abort(t_client_verbs *cv);
t_server_verbs	cmd_delete(t_client_verbs *cv);
t_server_verbs	cmd_remove_dir(t_client_verbs *cv);
t_server_verbs	cmd_make_dir(t_client_verbs *cv);
t_server_verbs	cmd_print_workdir(t_client_verbs *cv);
t_server_verbs	cmd_list(t_client_verbs *cv);
t_server_verbs	cmd_system(t_client_verbs *cv);
t_server_verbs	cmd_noop(t_client_verbs *cv);

int				response_to_client(int sock, t_ftp_code_enum code, char *description);

void			init_openssl();
void			cleanup_openssl();
SSL_CTX			*create_context();
void			configure_context(SSL_CTX *ctx);


#endif
