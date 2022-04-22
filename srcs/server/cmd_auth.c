/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_auth.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 16:07:54 by amineau           #+#    #+#             */
/*   Updated: 2022/04/22 20:51:32 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

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

t_server_verbs cmd_username(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_verbs        sv;
	const struct pam_conv conv = {&pamconv, NULL};

	(void)srv_ftp;
	if (!ft_strcmp(cv->cv_arg, ANONYMOUS_USER))
	{
		sv.sr_code = _230;
		sv.sr_state = POS_INT;
		sv.user_info = "Loggin succeded";
	}
	else if (pam_start("common", cv->cv_arg, &conv, &cv->cv_pamh) == PAM_SUCCESS)
	{
		sv.sr_code = _331;
		sv.sr_state = POS_INT;
		sv.user_info = ft_strjoin("Password for ", cv->cv_arg);
	}
	else
	{
		sv.sr_code = _530;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Unknown user";
	}
	return (sv);
}

t_server_verbs cmd_password(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_verbs        sv;
	int                   pam_status;
	const struct pam_conv conv = {&pamconv, (void *)cv->cv_arg};

	(void)srv_ftp;
	pam_set_item(cv->cv_pamh, PAM_CONV, (void *)&conv);
	pam_status =
		pam_authenticate(cv->cv_pamh, PAM_SILENT | PAM_DISALLOW_NULL_AUTHTOK);
	if (pam_status == PAM_SUCCESS)
	{
		sv.sr_code = _230;
		sv.sr_state = POS_INT;
		sv.user_info = "Loggin succeded";
	}
	else
	{
		sv.sr_code = _530;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Incorrect password";
	}
	pam_end(cv->cv_pamh, pam_status);
	return (sv);
}

t_server_verbs cmd_account(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	(void)cv;
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
			printf("Warning: SSL failed\n");
			ERR_print_errors_fp(stderr);
			shutdown_ssl(srv_ftp->pi.ssl);
			return false;
		}
	}
	else
	{
		ftp_srv_send_pi(&srv_ftp->pi, _234, "");
		exit(EXIT_SUCCESS);
	}
	return true;
}

t_server_verbs cmd_auth_method(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_verbs sv;

	if (!ft_strcmp(cv->cv_arg, "TLS"))
	{
		if (wait_for_ssl_client(srv_ftp))
		{
			srv_ftp->pi.ssl_activated = true;
			sv.sr_code = _NOCODE;
			sv.sr_state = POS_DEF;
			sv.user_info = "";
			return (sv);
		}
		else
		{
			// echec du serveur
		}
	}
	sv.sr_code = _520;
	sv.sr_state = NEG_DEF;
	sv.user_info = "Not supported";
	return (sv);
}

t_server_verbs cmd_protection_buffer_size(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_verbs sv;

	(void)srv_ftp;
	if (!cv->cv_arg || !ft_isnumber(cv->cv_arg))
	{
		sv.sr_code = _501;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Syntax error in the arguments";
	}
	else
	{
		sv.sr_code = _200;
		sv.sr_state = POS_DEF;
		sv.user_info = "";
	}
	return (sv);
}

t_server_verbs cmd_protection(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_verbs sv;
	char          *supported_params;

	supported_params = "CP";

	if (ft_strlen(cv->cv_arg) != 1 || !ft_strchr("CSEP", cv->cv_arg[0]))
	{
		sv.sr_code = _501;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Syntax error in the arguments";
	}
	else if (!ft_strchr(supported_params, cv->cv_arg[0]))
	{
		sv.sr_code = _536;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Not suppoted param";
	}
	else
	{
		srv_ftp->dtp.ssl_activated = true;
		sv.sr_code = _200;
		sv.sr_state = POS_DEF;
		sv.user_info = "";
	}
	return (sv);
}
