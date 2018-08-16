/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 20:07:43 by amineau           #+#    #+#             */
/*   Updated: 2018/08/16 02:12:27 by amineau          ###   ########.fr       */
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

# define MAX_PENDING_CONNECTIONS 42
# define MAX_CLIENT_CONNECTION 8

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

// static pthread_mutex_t	g_mutex_stock = PTHREAD_MUTEX_INITIALIZER;

const char*	commands_list[8] = {
	"ls",
	"cd",
	"get",
	"put",
	"pwd",
	"quit",
	"help",
	'\0'
};

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


#endif
