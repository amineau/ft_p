/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_generic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 04:38:25 by amineau           #+#    #+#             */
/*   Updated: 2022/04/17 10:34:29 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_server_verbs cmd_not_implemented(char *verb, t_srv_ftp *srv_ftp)
{
	t_server_verbs sv;

	(void)srv_ftp;
	sv.sr_code = _503;
	sv.sr_state = NEG_DEF;
	sv.user_info = ft_strcjoin(verb, "Not implemented command", ' ');
	return sv;
}