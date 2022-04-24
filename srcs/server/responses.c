/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responses.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 16:07:54 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 02:42:16 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include "ftp_srv_code_static.h"

int ftp_srv_send_pi(t_srv_transfert *srv_tranfert,
					t_ftp_code_enum  code,
					char            *description)
{
	char *str;
	int   ret;

	if (code == _NOCODE)
		return 0;
	str = ft_straddc(g_ftp_code_str[code], ' ');
	str = ft_strcln1join(ft_strcln1join(str, description), FTP_EOC);
	if (debug == true)
		ft_printf("\033[0;32mSERVER-PI: %s\033[0m", str);
	if (srv_tranfert->ssl_activated == true)
		ret = SSL_write(srv_tranfert->ssl, str, ft_strlen(str));
	else
		ret = write(srv_tranfert->cs, str, ft_strlen(str));

	free(str);
	return ret;
}

int ftp_srv_send_dtp(t_srv_transfert *srv_tranfert, char *str)
{
	int ret;

	if (debug == true)
		ft_printf("\033[0;36mSERVER-DTP : %s\n\033[0m", str);
	if (srv_tranfert->ssl_activated == true)
		ret = SSL_write(srv_tranfert->ssl, str, ft_strlen(str));
	else
		ret = write(srv_tranfert->cs, str, ft_strlen(str));
	return ret;
}

void ftp_srv_pipe_dtp_read(t_srv_transfert *srv_tranfert, int pfd)
{
	char buffer[BUFF_SIZE_DATA_READ + 1];
	int  r;

	while ((r = read(pfd, buffer, BUFF_SIZE_DATA_READ)) != 0)
	{
		buffer[r] = '\0';
		ftp_srv_send_dtp(srv_tranfert, buffer);
	}
}

void ftp_srv_pipe_dtp(t_srv_transfert *srv_tranfert,
					  const char      *path,
					  char *const      argv[])
{
	int pfd[2];
	int pid;

	if (pipe(pfd) == -1)
		ft_putendl("Error pipe failed.");
	if ((pid = fork()) < 0)
		ft_putendl("Error fork failed.");
	if (pid != 0)
	{
		close(pfd[1]);
		ftp_srv_pipe_dtp_read(srv_tranfert, pfd[0]);
		close(pfd[0]);
	}
	else
	{
		close(pfd[0]);
		if (dup2(pfd[1], STDOUT_FILENO) == -1)
			ft_putendl("Error redirect fd");
		if (execv(path, argv) == -1)
			ft_putendl("Exec format error.");
		close(pfd[1]);
		exit(EXIT_SUCCESS);
	}
}
