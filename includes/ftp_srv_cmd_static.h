/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_srv_cmd_static.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 12:47:47 by amineau           #+#    #+#             */
/*   Updated: 2022/05/02 20:08:31 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_SRV_CMD_STATIC_H
#define FTP_SRV_CMD_STATIC_H

#include "ft_p.h"

static const char *cmd_str[] = {
	[USERNAME] = "USER",
	[PASSWORD] = "PASS",
	[ACCOUNT] = "ACCT",
	[AUTH_METHOD] = "AUTH",
	[CHANGE_WORKDIR] = "CWD",
	[CHANGE_TO_PARENT_DIR] = "CDUP",
	[LOGOUT] = "QUIT",
	[PORT] = "PORT",
	[PASSIVE_MODE] = "PASV",
	[REPRESENTATION_TYPE] = "TYPE",
	[RETRIEVE] = "RETR",
	[STORE] = "STOR",
	[RENAME_FROM] = "RNFR",
	[RENAME_TO] = "RNTO",
	[ABORT] = "ABOR",
	[DELETE] = "DELE",
	[REMOVE_DIR] = "RMD",
	[MAKE_DIR] = "MKD",
	[PRINT_WORKDIR] = "PWD",
	[LIST] = "LIST",
	[SYSTEM] = "SYST",
	[PROTECTION_BUFFER_SIZE] = "PBSZ",
	[PROTECTION] = "PROT",
	[MODE] = "MODE",
	[STRUCTURE] = "STRU",
	[NOOP] = "NOOP",
	NULL,
};

#endif