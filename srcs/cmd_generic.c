/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_generic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 04:38:25 by amineau           #+#    #+#             */
/*   Updated: 2018/08/17 04:41:25 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_server_verbs	cmd_not_implemented(void)
{
	t_server_verbs	sv;

	sv.sr_code = _202;
	sv.sr_state = POS_DEF;
	sv.user_info = "Not implemented command";
	return sv;
}