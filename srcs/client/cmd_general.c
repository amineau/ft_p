/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_general.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 00:16:01 by amineau           #+#    #+#             */
/*   Updated: 2022/04/23 21:43:03 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"
#include "ftp_srv_cmd_static.h"

void put_req_arg(t_cmd cmd)
{
	ft_printf("Argument is required for this command [%s]\n", cmd_str[cmd]);
}

void put_no_req_arg(t_cmd cmd)
{
	ft_printf("This command [%s] is used without arguments\n", cmd_str[cmd]);
}

int ftp_cli_cmd_help(t_cli_ftp *cli_ftp, const char *args)
{
	(void)args;
	(void)cli_ftp;
	ft_printf("ls,cd,get,put,pwd,quit,help\n");
	return 1;
}