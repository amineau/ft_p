/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_auth.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 16:07:54 by amineau           #+#    #+#             */
/*   Updated: 2019/02/10 05:14:49 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

// TODO : Insert to libft
void *ft_calloc(size_t n, size_t size)
{
	size_t total = n * size;
	void *p = malloc(total);
	
	if (!p) return NULL;
	
	return ft_memset(p, 0, total);
}

static int pamconv(int num_msg, const struct pam_message **msg,
        struct pam_response **resp, void *appdata_ptr)
{
    int i;
    char *pass;
    
    pass = (char*)malloc(ft_strlen(appdata_ptr) + 1);
    ft_strcpy(pass, appdata_ptr);
    *resp = (struct pam_response*)ft_calloc(num_msg, sizeof(struct pam_response));
    i = 0;
    while (i < num_msg)
    {
        if (msg[i]->msg_style != PAM_PROMPT_ECHO_OFF)
            continue;
        resp[i++]->resp = pass;
    }
    return PAM_SUCCESS;
}

t_server_verbs  cmd_username(t_client_verbs* cv, t_env* env)
{
    t_server_verbs          sv;
    const struct pam_conv   conv = {
        &pamconv,
        NULL
    };

    if (pam_start("common", cv->cv_arg, &conv, &cv->cv_pamh) == PAM_SUCCESS)
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

t_server_verbs  cmd_password(t_client_verbs* cv, t_env* env)
{
    t_server_verbs  sv;
    int             pam_status;
    const struct pam_conv   conv = {
        &pamconv,
        (void *)cv->cv_arg
    };

    pam_set_item(cv->cv_pamh, PAM_CONV, (void*)&conv);
    pam_status = pam_authenticate(cv->cv_pamh,
        PAM_SILENT|PAM_DISALLOW_NULL_AUTHTOK);
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

t_server_verbs  cmd_account(t_client_verbs* cv, t_env* env)
{
    (void)cv;
    return (cmd_not_implemented(ACCOUNT, env));
}

t_server_verbs  cmd_auth_method(t_client_verbs* cv, t_env* env)
{
    t_server_verbs  sv;

    if (!ft_strcmp(cv->cv_arg, "TLS"))
    {
        printf("DEBUG FLAG 1\n");
        env->ssl = SSL_new(*env->ctx);
        printf("ssl second : %p\n", env->ssl);
        SSL_set_fd(env->ssl, env->cs);
        response_to_client(env, _234, "");
        // TODO : Probablement qu'il faut recreer une socket propre au ssl
        if (SSL_accept(env->ssl) <= 0)
        {
            printf("DEBUG FLAG 2\n");
            ERR_print_errors_fp(stderr);
        }
        else
        {
            printf("DEBUG FLAG 3\n");
            env->ssl_activated = true;
            sv.sr_code = _332;
            sv.sr_state = POS_DEF;
            sv.user_info = TLS_VERSION;
            return (sv);
        }
    }
    sv.sr_code = _520;
    sv.sr_state = NEG_DEF;
    sv.user_info = "Not supported";
    return (sv);
}