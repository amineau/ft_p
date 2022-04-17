/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_transfert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 16:07:54 by amineau           #+#    #+#             */
/*   Updated: 2022/04/17 20:45:11 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_server_verbs  cmd_port(t_client_verbs* cv, t_srv_ftp* srv_ftp)
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


char * addrtostr(in_addr_t s_addr)
{
    char**  addr;
    char*   str;

    addr = (char *[]){
        ft_itoa((s_addr & 0x000000FF)),
        ft_itoa((s_addr & 0x0000FF00) >> 8),
        ft_itoa((s_addr & 0x00FF0000) >> 16),
        ft_itoa((s_addr & 0xFF000000) >> 24),
        NULL,
    };
    str = ft_arraycjoin(addr, ',');
    // ft_arrayfree(addr);
    return str;
}

char * porttostr(in_port_t sin_port)
{
    char**   port;
    char*   str;


    port = (char *[]){
        ft_itoa((sin_port & 0x00FF)),
        ft_itoa((sin_port & 0xFF00) >> 8),
        NULL
    };

    str = ft_arraycjoin(port, ',');
    // ft_arrayfree(port);
    return str;
}


t_server_verbs  cmd_passive_mode(t_client_verbs* cv, t_srv_ftp* srv_ftp)
{
    t_server_verbs      sv;
    int                 sock;
    struct sockaddr_in  sin;
    socklen_t           sinlen;         
    char*               port;
    char*               addr;

    sinlen = sizeof(sin);
    srv_ftp->dtp.sock = ftp_create_channel(5666);
    if (getsockname(srv_ftp->dtp.sock, (struct socksin_in*)&sin, &sinlen) != 0)
    {
        sv.sr_code = _421;
        sv.sr_state = NEG_DEF;
        sv.user_info = "Cannot open data connection.";
        return (sv);
    }
    if (srv_ftp->debug)
        printf("PORT-DTP: %d\n", ntohs(sin.sin_port));
    addr = addrtostr(sin.sin_addr.s_addr);
    port = porttostr(sin.sin_port);
    sv.sr_code = _227;
    sv.sr_state = POS_INT;
    sv.user_info = ft_arrayjoin((char *[]){
        "Entering Passive Mode. ",
        ft_arraycjoin((char *[]){addr, port, NULL}, ','),
        NULL}
    );
    free(port);
    free(addr);
    return (sv);
}
