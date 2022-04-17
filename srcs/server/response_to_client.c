/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_srv_send_pi.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 16:07:54 by amineau           #+#    #+#             */
/*   Updated: 2022/04/17 19:46:45 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int			ftp_srv_send_pi(t_srv_ftp* srv_ftp, t_ftp_code_enum code, char *description)
{
	char	*str;
	int		ret;

	if (code == _NOCODE)
		return 0;
	str = ft_straddc(g_ftp_code_str[code], ' ');
	str = ft_strcln1join(ft_strcln1join(str, description), FTP_EOC);
	if (srv_ftp->debug == true)
		ft_printf("SERVER-PI : %s", str);
	if (srv_ftp->ssl_activated == true)
		ret = SSL_write(srv_ftp->pi.ssl, str, ft_strlen(str));
	else
		ret = write(srv_ftp->pi.cs, str, ft_strlen(str));
	
	ERR_print_errors_fp(stderr);	
	free(str);
	return ret;
}

int			ftp_srv_send_dtp(t_srv_ftp* srv_ftp, char *data, int len)
{
	int		ret;
	char 	*str;

	str = ft_strjoin(data, FTP_EOC);

	if (srv_ftp->debug == true)
		ft_printf("SERVER-DTP : %s\n", str);
	if (srv_ftp->ssl_activated == true)
		ret = SSL_write(srv_ftp->dtp.ssl, str, ft_strlen(str));
	else
		ret = write(srv_ftp->dtp.cs, str, ft_strlen(str));
	ERR_print_errors_fp(stderr);	
	return ret;
}