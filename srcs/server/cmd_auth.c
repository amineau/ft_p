/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_auth.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 16:07:54 by amineau           #+#    #+#             */
/*   Updated: 2022/04/26 13:37:57 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

// TODO : Insert to libft
void *ft_calloc(size_t n, size_t size)
{
	size_t total = n * size;
	void  *p = malloc(total);

	if (!p)
		return NULL;

	return ft_memset(p, 0, total);
}

static int pamconv(int                        num_msg,
				   const struct pam_message **msg,
				   struct pam_response      **resp,
				   void                      *appdata_ptr)
{
	int   i;
	char *pass;

	pass = (char *)malloc(ft_strlen(appdata_ptr) + 1);
	ft_strcpy(pass, appdata_ptr);
	*resp =
		(struct pam_response *)ft_calloc(num_msg, sizeof(struct pam_response));
	i = 0;
	while (i < num_msg)
	{
		if (msg[i]->msg_style != PAM_PROMPT_ECHO_OFF)
			continue;
		resp[i++]->resp = pass;
	}
	return PAM_SUCCESS;
}

t_srv_res cmd_username(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res             response;
	const struct pam_conv conv = {&pamconv, NULL};

	if (!ft_strcasecmp(req->req_arg, ANONYMOUS_USER))
		response = ftp_build_srv_res(_230, "Loggin succeded");
	else if (pam_start("common", req->req_arg, &conv, &srv_ftp->conf.pamh) ==
			 PAM_SUCCESS)
		response =
			ftp_build_srv_res(_331, ft_strjoin("Password for ", req->req_arg));
	else
		response = ftp_build_srv_res(_530, "Unknown user");
	return (response);
}

t_srv_res cmd_password(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res             response;
	int                   pam_status;
	const struct pam_conv conv = {&pamconv, (void *)req->req_arg};

	pam_set_item(srv_ftp->conf.pamh, PAM_CONV, (void *)&conv);
	pam_status = pam_authenticate(srv_ftp->conf.pamh,
								  PAM_SILENT | PAM_DISALLOW_NULL_AUTHTOK);
	if (pam_status == PAM_SUCCESS)
		response = ftp_build_srv_res(_230, "Loggin succeded");
	else
		response = ftp_build_srv_res(_530, "Incorrect password");
	pam_end(srv_ftp->conf.pamh, pam_status);
	return (response);
}

t_srv_res cmd_account(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	(void)req;
	return (cmd_not_implemented(ACCOUNT, srv_ftp));
}

int wait_for_ssl_client(t_srv_ftp *srv_ftp)
{
	int pid;

	srv_ftp->pi.ssl = SSL_new(*srv_ftp->ctx);
	SSL_set_fd(srv_ftp->pi.ssl, srv_ftp->pi.cs);
	pid = fork();
	if (pid != 0)
	{
		if (SSL_accept(srv_ftp->pi.ssl) <= 0)
		{
			ft_printf("Warning: SSL failed\n");
			shutdown_ssl(srv_ftp->pi.ssl);
			return false;
		}
	}
	else
	{
		ftp_srv_response_pi(&srv_ftp->pi, _234, "");
		exit(EXIT_SUCCESS);
	}
	return true;
}

t_srv_res cmd_auth_method(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;

	if (!ft_strcmp(req->req_arg, "TLS"))
	{
		if (wait_for_ssl_client(srv_ftp))
		{
			srv_ftp->pi.ssl_activated = true;
			response = ftp_build_srv_res(_NOCODE, "");
		}
		else
		{
			// echec du serveur
		}
	}
	else
		response = ftp_build_srv_res(_520, "Not supported");
	return (response);
}

t_srv_res cmd_protection_buffer_size(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;

	(void)srv_ftp;
	if (!req->req_arg || !ft_isnumber(req->req_arg))
		response = ftp_build_srv_res(_501, "Syntax error in the arguments");
	else
		response = ftp_build_srv_res(_200, "");
	return (response);
}

t_srv_res cmd_protection(t_cli_req *req, t_srv_ftp *srv_ftp)
{
	t_srv_res response;
	char     *supported_params;

	supported_params = "CP";

	if (ft_strlen(req->req_arg) != 1 || !ft_strchr("CSEP", req->req_arg[0]))
		response = ftp_build_srv_res(_501, "Syntax error in the arguments");

	else if (!ft_strchr(supported_params, req->req_arg[0]))
		response = ftp_build_srv_res(_536, "Not suppoted param");
	else
	{
		srv_ftp->dtp.ssl_activated = true;
		response = ftp_build_srv_res(_200, "");
	}
	return (response);
}
