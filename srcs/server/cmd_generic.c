/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_generic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 04:38:25 by amineau           #+#    #+#             */
/*   Updated: 2018/08/18 13:23:50 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_server_verbs	cmd_not_implemented(char* verb, t_env* env)
{
	t_server_verbs	sv;

	(void)env;
	sv.sr_code = _503;
	sv.sr_state = NEG_DEF;
	sv.user_info = ft_strcjoin(verb, "Not implemented command", ' ');
	return sv;
}