/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_transfert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 16:07:54 by amineau           #+#    #+#             */
/*   Updated: 2019/02/10 03:44:22 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_server_verbs  cmd_port(t_client_verbs* cv, t_env* env)
{
    t_server_verbs  sv;
    int             i;
    char**          transfert_port_str;
    int             transfert_port[6];

    transfert_port_str = ft_strsplit(cv->cv_arg, ',');
    i = 0;
    while(transfert_port_str[i])
    {
        transfert_port[i] = ft_atoi(transfert_port_str[i]);
        i++;
    }
    sv.sr_code = _200;
    sv.sr_state = POS_INT;
    sv.user_info = "Ok";
    return (sv);
}

t_server_verbs  cmd_passive_mode(t_client_verbs* cv, t_env* env)
{
    (void)cv;
    return (cmd_not_implemented(PASSIVE_MODE, env));
}
