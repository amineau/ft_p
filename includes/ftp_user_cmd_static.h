/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_user_cmd_static.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 12:47:47 by amineau           #+#    #+#             */
/*   Updated: 2022/04/23 21:39:39 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_USER_CMD_STATIC_H
#define FTP_USER_CMD_STATIC_H

#include "ft_p.h"

static const char *g_user_cmd_str[] = {
	"ls", "cd", "get", "put", "pwd", "quit", "help", NULL};

#endif