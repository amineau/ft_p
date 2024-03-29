/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_user_cmd_static.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 12:47:47 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 13:12:46 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_USER_CMD_STATIC_H
#define FTP_USER_CMD_STATIC_H

#include "ftp_client.h"

static const char *user_cmd_str[] = {[LLS] = "lls",
									 [LCD] = "lcd",
									 [LPWD] = "lpwd",
									 [LS] = "ls",
									 [CD] = "cd",
									 [GET] = "get",
									 [PUT] = "put",
									 [PWD] = "pwd",
									 [QUIT] = "quit",
									 [HELP] = "help",
									 NULL};

#endif