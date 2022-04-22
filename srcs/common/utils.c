/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 16:48:18 by amineau           #+#    #+#             */
/*   Updated: 2022/04/22 21:39:55 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"
#include "ftp_srv_code_static.h"

int ftp_is_valid_response_code(char *code)
{
	int i;

	i = 1;
	if (ft_strlen(code) == 3)
	{
		if (code[0] >= '1' && code[0] <= '5')
		{
			while (code[i])
				if (!ft_isdigit(code[i++]))
					return 0;
			return 1;
		}
	}
	return 0;
}

t_state ftp_get_state_code(char *code)
{
	return code[0] - '0';
}

t_ftp_code_enum ftp_get_ftp_code_enum(const char *code)
{
	int i;

	i = 0;
	while (g_ftp_code_str[i] && ft_strcmp(g_ftp_code_str[i], code))
		i++;
	return i;
}

struct in_addr htoaddr(char *name)
{
	struct hostent *host;
	struct in_addr  addr;
	char           *err;

	host = gethostbyname(name);
	if (!host)
	{
		if (h_errno == HOST_NOT_FOUND)
			err = "Host is unknown";
		else if (h_errno == NO_DATA)
			err = "Host does not have an IP address";
		else if (h_errno == NO_RECOVERY)
			err = "Server error";
		else if (h_errno == TRY_AGAIN)
			err = "A temporary error occurred on an authoritative name server.  Try again later.";
		else
			err = "Host by name has failed";
		error_print_exit(EXIT_FAILURE, err);
	}
	ft_memcpy(&addr.s_addr, host->h_addr, host->h_length);
	return (addr);
}

struct in_addr stoaddr(in_addr_t s_addr)
{
	struct in_addr addr;

	addr.s_addr = s_addr;
	return (addr);
}
