/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_to_client.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 16:07:54 by amineau           #+#    #+#             */
/*   Updated: 2018/08/17 06:57:40 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int			response_to_client(int sock, t_ftp_code_enum code, char *description)
{
	printf("DEBUG : %s %s%s", g_ftp_code_str[code], description, FTP_EOC);
	return(ft_printf_fd(sock, "%s %s%s", g_ftp_code_str[code], description, FTP_EOC));
}