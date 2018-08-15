/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 20:07:43 by amineau           #+#    #+#             */
/*   Updated: 2018/08/15 22:40:00 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_P_H
# define FT_P_H

# define MAX_CONNECTION 42

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>

// Debug
# include <string.h>
// End Debug
# include "libft.h"

typedef struct	s_client_args
{
	struct in_addr	ca_host;
	int				ca_port;
	char*			ca_user;
	char*			ca_pass;
	char*			ca_dir;
}				t_client_args;

#endif
