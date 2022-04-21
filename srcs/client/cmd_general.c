/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_general.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 00:16:01 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 01:34:56 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void put_req_arg(char *cmd)
{
	printf("Argument is required for this command [%s]\n", cmd);
}

void put_no_req_arg(char *cmd)
{
	printf("This command [%s] is used without arguments\n", cmd);
}

int ftp_cli_cmd_help(t_cli_ftp *cli_ftp, const char *args)
{
	(void)args;
	(void)cli_ftp;
	printf("ls,cd,get,put,pwd,quit,help\n");
	return 1;
}