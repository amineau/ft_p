/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_srv_cmd_static.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 12:47:47 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 12:56:08 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_SRV_CMD_STATIC_H
#define FTP_SRV_CMD_STATIC_H

#include "ft_p.h"

static const char *g_ftp_cmd_str[] = {
	USERNAME,
	PASSWORD,
	ACCOUNT,
	AUTH_METHOD,
	CHANGE_WORKDIR,
	CHANGE_TO_PARENT_DIR,
	LOGOUT,
	PORT,
	PASSIVE_MODE,
	REPRESENTATION_TYPE,
	RETRIEVE,
	STORE,
	RENAME_FROM,
	RENAME_TO,
	ABORT,
	DELETE,
	REMOVE_DIR,
	MAKE_DIR,
	PRINT_WORKDIR,
	LIST,
	SYSTEM,
	PROTECTION_BUFFER_SIZE,
	PROTECTION,
	NOOP,
	NULL};

#endif