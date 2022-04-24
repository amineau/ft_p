/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_local.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 01:39:42 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 02:08:15 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"
// #include "ftp_user_cmd_static.h"

int ftp_cli_cmd_local_change_directory(t_cli_ftp *cli_ftp, const char *dirname)
{
	(void)cli_ftp;

	if (chdir(dirname) == -1)
	{
		ft_printf("Change directory has failed");
		return 0;
	}
	ft_printf("%s\n", ft_getcwd());
	return 1;
}

int ftp_cli_cmd_local_print_directory(t_cli_ftp *cli_ftp, const char *args)
{
	(void)cli_ftp;
	(void)args;

	ft_printf("%s\n", ft_getcwd());
	return 1;
}

int ftp_cli_cmd_local_list(t_cli_ftp *cli_ftp, const char *args)
{
	pid_t pid;

	(void)cli_ftp;

	if (!(pid = fork()))
		execl("/bin/ls", "ls", "-l", args, (char *)NULL);
	else
		wait4(pid, 0, 0, 0);
	return 1;
}