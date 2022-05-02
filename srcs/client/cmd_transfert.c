/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_transfert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 00:43:07 by amineau           #+#    #+#             */
/*   Updated: 2022/04/30 11:49:19 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"
#include "ftp_srv_cmd_static.h"

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

int count_numbers(const char *str)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if ((ft_isdigit(str[i]) && i == 0) ||
			(i > 0 && ft_isdigit(str[i]) && !ft_isdigit(str[i - 1])))
			count++;
		i++;
	}
	return (count);
}

// First int is length
int *get_numbers(const char *str)
{
	int *numbers;
	int  count;
	int  i;

	count = count_numbers(str);
	numbers = (int *)malloc(sizeof(int) * (count + 1));

	numbers[0] = count;
	i = 0;
	while (str[i])
	{
		if ((ft_isdigit(str[i]) && i == 0) ||
			(i > 0 && ft_isdigit(str[i]) && !ft_isdigit(str[i - 1])))
			numbers[numbers[0] - --count] = ft_atoi(&str[i]);
		i++;
	}
	return (numbers);
}

int ftp_cli_cmd_passive(t_cli_ftp *cli_ftp)
{
	t_srv_res *srv_verbs;
	int       *numbers;

	ftp_cli_send_pi(cli_ftp, cmd_str[PASSIVE_MODE], NULL);
	srv_verbs = ftp_wait_for_response(cli_ftp);
	if (ftp_get_state_code(srv_verbs->sr_code) == POS_DEF)
	{
		numbers = get_numbers(get_last_word(srv_verbs->user_info));
		if (numbers[0] == 6)
			cli_ftp->dtp.sin = ftp_get_socket_address(
				stoaddr(itoaddr(&(numbers[1]))), htons(itoport(&(numbers[5]))));
		else
			error_print_exit(EXIT_FAILURE, "Unable to parse response server");
	}
	else if (ftp_get_state_code(srv_verbs->sr_code) == NEG_TMP)
		exit(EXIT_FAILURE_RETRY);
	else
		exit(EXIT_FAILURE);
	return (1);
}