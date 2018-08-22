/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_files_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 04:42:08 by amineau           #+#    #+#             */
/*   Updated: 2018/08/18 13:22:44 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_server_verbs	cmd_print_workdir(t_client_verbs* cv, t_env* env)
{
	t_server_verbs	sv;
	char*			wdir;
	char			buff[BUFF_SIZE];

	if (cv->cv_arg)
	{
		sv.sr_code = _504;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Command without param only";
	}
	else
	{
		wdir = get_wdir();
		printf("**%s**",wdir);
		sprintf(buff, "\"%s\" is the current directory", wdir);
		ft_strdel(&wdir);
		sv.sr_code = _257;
		sv.sr_state = POS_DEF;
		sv.user_info = buff;
	}
	return (sv);
}

t_server_verbs	cmd_change_workdir(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(CHANGE_WORKDIR, env));
} 
t_server_verbs	cmd_change_to_parent_dir(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(CHANGE_TO_PARENT_DIR, env));
} 
t_server_verbs	cmd_logout(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(LOGOUT, env));
}
t_server_verbs	cmd_representation_type(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(REPRESENTATION_TYPE, env));
}
t_server_verbs	cmd_retrieve(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(RETRIEVE, env));
} 
t_server_verbs	cmd_store(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(STORE, env));
} 
t_server_verbs	cmd_rename_from(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(RENAME_FROM, env));
} 
t_server_verbs	cmd_rename_to(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(RENAME_TO, env));
} 
t_server_verbs	cmd_abort(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(ABORT, env));
} 
t_server_verbs	cmd_delete(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(DELETE, env));
} 
t_server_verbs	cmd_remove_dir(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(REMOVE_DIR, env));
} 
t_server_verbs	cmd_make_dir(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(MAKE_DIR, env));
} 
t_server_verbs	cmd_list(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(LIST, env));
} 
t_server_verbs	cmd_system(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(SYSTEM, env));
} 
t_server_verbs	cmd_noop(t_client_verbs* cv, t_env* env)
{
	(void)cv;
    return (cmd_not_implemented(NOOP, env));
}