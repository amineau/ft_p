/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_generic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 04:38:25 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 14:20:59 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include "ftp_srv_cmd_static.h"

t_srv_res cmd_not_implemented(t_cmd cmd, t_srv_ftp *srv_ftp)
{
	(void)srv_ftp;
	return (ftp_build_srv_res(
		_503, ft_strcjoin(cmd_str[cmd], "Not implemented command", ' ')));
}