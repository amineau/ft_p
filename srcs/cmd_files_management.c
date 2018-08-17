/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_files_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 04:42:08 by amineau           #+#    #+#             */
/*   Updated: 2018/08/17 07:04:35 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

t_server_verbs	cmd_print_workdir(t_client_verbs *cv)
{
	t_server_verbs	sv;
	char*			wdir;
	char			buff[1024];

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
		ft_strcpy(buff, wdir);
		ft_strdel(&wdir);
		sv.sr_code = _257;
		sv.sr_state = POS_DEF;
		sv.user_info = buff;
	}
	return (sv);
}

t_server_verbs	cmd_change_workdir(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_change_to_parent_dir(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_logout(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_retrieve(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_store(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_rename_from(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_rename_to(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_abort(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_delete(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_remove_dir(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_make_dir(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_list(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_system(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
} 
t_server_verbs	cmd_noop(t_client_verbs *cv)
{
	(void)cv;
    return (cmd_not_implemented());
}