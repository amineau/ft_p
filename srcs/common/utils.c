/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 16:48:18 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 12:55:52 by amineau          ###   ########.fr       */
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

	host = gethostbyname(name);
	if (!host)
	{
		if (h_errno == HOST_NOT_FOUND)
			printf("%s is unknown\n", name);
		else if (h_errno == NO_DATA)
			printf("%s does not have an IP address\n", name);
		else if (h_errno == NO_RECOVERY)
			printf("Server error\n");
		else if (h_errno == TRY_AGAIN)
			printf("A temporary error occurred on an authoritative name server.  Try again later.\n");
		exit(EXIT_FAILURE);
	}
	ft_memcpy(&addr.s_addr, host->h_addr, host->h_length);
	return (addr);
}