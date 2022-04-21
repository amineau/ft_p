/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_transfert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 00:43:07 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 01:34:56 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

char *get_last_word(const char *str)
{
	char **split;
	int    i;

	split = ft_strsplit(str, ' ');
	if (!split)
		return (NULL);
	i = 0;
	while (split[i + 1])
		i++;
	return (split[i]);
}

size_t count_numbers(const char *str)
{
	size_t i;
	size_t count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if ((ft_isdigit(str[i] && i == 0) ||
			 (ft_isdigit(str[i] && !ft_isdigit(str[i - 1])))))
			count++;
		i++;
	}
	return (count);
}

// First int is length
size_t *get_numbers(const char *str)
{
	size_t *numbers;
	size_t  count;
	size_t  i;

	count = count_numbers(str);
	numbers = (size_t *)malloc(sizeof(size_t) * (count + 1));

	numbers[0] = count;
	while (str[i])
	{
		if ((ft_isdigit(str[i] && i == 0) ||
			 (ft_isdigit(str[i] && !ft_isdigit(str[i - 1])))))
			numbers[numbers[0] - --count] = ft_atoi(str[i]);
		i++;
	}
}

in_addr_t itoaddr(size_t *numbers)
{
	return (in_addr_t)((numbers[0] << 24) | (numbers[1] << 16) | (numbers[2] << 8) | numbers[3]);
}

in_port_t itoaport(size_t *numbers)
{
	return (in_port_t)((numbers[0] << 8) | numbers[1]);
}

int ftp_cli_cmd_passive_mode(t_cli_ftp *cli_ftp)
{
	t_server_verbs    *srv_verbs;
	int               *numbers;
	struct protoent   *proto;
	struct sockaddr_in sin;

	ftp_cli_send_pi(cli_ftp, PASSIVE_MODE, NULL);
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (srv_verbs->sr_state == POS_DEF)
	{
		numbers = get_numbers(get_last_word(srv_verbs->user_info));
		if (numbers[0] == 6)
		{
			proto = getprotobyname("tcp");
			if (!proto)
				exit(EXIT_FAILURE);
			cli_ftp->dtp.sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
			sin.sin_family = AF_INET;
			sin.sin_port = htons(itoaport(&(numbers[5])));
			sin.sin_addr.s_addr = itoaddr(&(numbers[1]));
		}
	}
	else if (srv_verbs->sr_state == NEG_TMP)
		exit(EXIT_FAILURE_RETRY);
	else
		exit(EXIT_FAILURE);
	return (1);
}