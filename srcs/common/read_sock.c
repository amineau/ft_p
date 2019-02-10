/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_sock.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2019/02/10 04:56:57 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_bool	is_valid_tls(char *str, size_t len)
{
	return ((!ft_strcmp(&str[len - 2], CRLF)));
}

char*	_received(t_env *env)
{
	char	buff[BUFF_SIZE];
	char*	ptr;
	char*	ret;
	int		r;

	if (env->ssl_activated == true)
		r = SSL_read(env->ssl, buff, BUFF_SIZE - 1);
	else
		r = read(env->cs, buff, BUFF_SIZE - 1);
	ERR_print_errors_fp(stderr);
	if (r <=0)
		return (NULL);
	buff[r] = '\0';

	if (r != 0 && !is_valid_tls(buff, r))
	{
		ret = _received(env);
		ft_strncat(buff, ret, BUFF_SIZE);
		ft_strdel(&ret);
	}
	ptr = (char*)malloc(sizeof(char) * (ft_strlen(buff) + 1));
	ft_strcpy(ptr, buff);
	return (ptr);
}

int		received(t_env *env, char* buff)
{
	char*	ret;

	buff[0] = '\0';
	ret = _received(env);
	ft_strcpy(buff, ret);
	ft_strdel(&ret);
	if (env->debug == true)
		ft_printf("Received : %s", buff);
	return ft_strlen(buff);
}