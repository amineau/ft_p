/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_generic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 04:38:25 by amineau           #+#    #+#             */
/*   Updated: 2022/04/23 18:03:09 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"
#include "ftp_srv_cmd_static.h"

t_server_verbs cmd_not_implemented(t_cmd cmd, t_srv_ftp *srv_ftp)
{
	t_server_verbs sv;

	(void)srv_ftp;
	sv.sr_code = _503;
	sv.sr_state = NEG_DEF;
	sv.user_info = ft_strcjoin(cmd_str[cmd], "Not implemented command", ' ');
	return sv;
}