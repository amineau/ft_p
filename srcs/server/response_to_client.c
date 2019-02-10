/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_to_client.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 16:07:54 by amineau           #+#    #+#             */
/*   Updated: 2019/02/10 04:35:36 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

int			response_to_client(t_env* env, t_ftp_code_enum code, char *description)
{
	char	*str;
	int		ret;

	str = ft_straddc(g_ftp_code_str[code], ' ');
	str = ft_strcln1join(ft_strcln1join(str, description), FTP_EOC);
	if (env->debug == true)
		ft_printf("Command : %s", str);
	if (env->ssl_activated == true)
		ret = SSL_write(env->ssl, str, ft_strlen(str));
	else
		ret = write(env->cs, str, ft_strlen(str));
	ERR_print_errors_fp(stderr);	
	free(str);
	return ret;
}