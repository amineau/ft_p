/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 20:07:43 by amineau           #+#    #+#             */
/*   Updated: 2018/08/16 18:05:31 by amineau          ###   ########.fr       */
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

// Debug
# include <string.h>
// End Debug
# include "libft.h"


# define GENERATE_ENUM(ENUM) ENUM
# define GENERATE_CODE_STRING(STRING) &#STRING[1]
# define GENERATE_STRING(STRING) #STRING

# define MAX_PENDING_CONNECTIONS 42
# define MAX_CLIENT_CONNECTION 8

# define CRLF "\r\n"
# define LF '\n'
# define NUL '\0'

# define FTP_EOC CRLF

/********** FTP COMMANDS **********/
# define USERNAME "USER"
# define PASSWORD "PASS"
# define ACCOUNT  "ACCT"
# define CHANGE_WORKDIR "CWD"
# define CHANGE_TO_PARENT_DIR "CDUP"
# define LOGOUT "QUIT"
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
# define FOREACH_FTP_CODE(FTP_CODE) \
		 FTP_CODE(_120), \
		 FTP_CODE(_150), \
		 FTP_CODE(_200), \
		 FTP_CODE(_202), \
		 FTP_CODE(_211), \
		 FTP_CODE(_214), \
		 FTP_CODE(_220), \
		 FTP_CODE(_221), \
		 FTP_CODE(_226), \
		 FTP_CODE(_230), \
		 FTP_CODE(_250), \
		 FTP_CODE(_257), \
		 FTP_CODE(_125), \
		 FTP_CODE(_331), \
		 FTP_CODE(_332), \
		 FTP_CODE(_350), \
		 FTP_CODE(_421), \
		 FTP_CODE(_425), \
		 FTP_CODE(_426), \
		 FTP_CODE(_450), \
		 FTP_CODE(_451), \
		 FTP_CODE(_452), \
		 FTP_CODE(_500), \
		 FTP_CODE(_501), \
		 FTP_CODE(_502), \
		 FTP_CODE(_503), \
		 FTP_CODE(_504), \
		 FTP_CODE(_506), \
		 FTP_CODE(_530), \
		 FTP_CODE(_532), \
		 FTP_CODE(_550), \
		 FTP_CODE(_551), \
		 FTP_CODE(_552), \
		 FTP_CODE(_553)


typedef enum	e_ftp_code
{
    FOREACH_FTP_CODE(GENERATE_ENUM)
}				t_ftp_code;

static const char *g_ftp_code_str[] = {
    FOREACH_FTP_CODE(GENERATE_CODE_STRING)
};

/**********************************/


// static pthread_mutex_t	g_mutex_stock = PTHREAD_MUTEX_INITIALIZER;

static const char*	g_ftp_cmd_str[] = {
	USERNAME,
	PASSWORD,
	ACCOUNT,
	CHANGE_WORKDIR,
	CHANGE_TO_PARENT_DIR,
	LOGOUT,
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
	NUL
};

static const char*	g_user_cmd_str[] = {
	"ls",
	"cd",
	"get",
	"put",
	"pwd",
	"quit",
	"help",
	NUL
};

typedef enum	e_bool
{
	false,
	true
}				t_bool;

typedef enum	e_state
{
	POS_TEMP,
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
}				t_client_args;

typedef struct	s_server_args
{
	int				ca_port;
	char*			ca_dir;
}				t_server_args;

typedef struct	s_client_verbs
{
	char*	cv_verb;
	char*	cv_arg;
	int		cv_code;
}				t_client_verbs;

typedef struct	s_server_verbs
{
	char*	sr_code;
	t_state	sr_state;
	char*	user_info;
}				t_server_verbs;

typedef char *(*t_action)(t_client_verbs*);

char    *ftp_username(t_client_verbs *cv);


#endif
