/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2018/08/17 07:20:40 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void	usage(char *str)
{
	ft_printf("Usage: %s <port> [-r <root directory>]\n", str);
	exit(-1);
}

int		create_server(int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (!proto)
		exit(-1);
	sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		if (errno == EACCES)
			ft_printf("This address is protected\n");
		else if(errno == EADDRINUSE)
			ft_printf("This address is already in use\n");
		else
			ft_printf("Bind failed");
		exit(-1);
	}
	if (listen(sock, MAX_PENDING_CONNECTIONS) == -1)
	{
		if (errno == ECONNREFUSED)
			ft_printf("The queue is full");
		else
			ft_printf("Listen failed");
		exit(-1);
	}
	return(sock);
}

int		open_client(int sock)
{
	int					cs;
	struct sockaddr_in	csin;
	unsigned int		cslen;

	errno = 0;
	if ((cs = accept(sock, (struct sockaddr *)&csin, &cslen)) == -1)
	{
		if (errno == EBADF)
			ft_printf("The file descriptor is invalid\n");
		else if (errno == ECONNABORTED)
			ft_printf("The connection has been aborted\n");
		else
			ft_printf("Accept failed\n\terrno : %d\n\terror : %s\n", errno, strerror(errno));
	}
	response_to_client(cs, _220, "Server available for new user");
	return (cs);
}

// TODO : Merge with user_lexer (client.c)
t_server_verbs	ftp_lexer(const char *buff, t_client_verbs* cv)
{
	char**			split;
	int				code_command;
	t_server_verbs	sv;

	split = ft_strsplit(buff, ' ');
	cv->cv_verb = ft_strrtrim(split[0]);
	printf("SPLIT DEBUG : [%s]\n", cv->cv_verb);
	if (!cv->cv_verb || (code_command = ft_arraystr(g_ftp_cmd_str, cv->cv_verb)) == -1)
	{
		sv.sr_code = _500;
		sv.sr_state = NEG_DEF;
		sv.user_info = "Unknown command";
		return (sv);
	}
	cv->cv_arg = ft_strrtrim(split[1]);
	cv->cv_code = code_command;
	sv.sr_code = _100;
	sv.sr_state = POS_TMP;
	sv.user_info = "";
	return (sv);
}

// TODO : Merge with user_parser (client.c)
t_server_verbs	ftp_parser(t_client_verbs* cv)
{
	t_server_action	command[] = {
		cmd_username, cmd_password, cmd_account, cmd_change_workdir,
		cmd_change_to_parent_dir, cmd_logout, cmd_retrieve, cmd_store,
		cmd_rename_from, cmd_rename_to, cmd_abort, cmd_delete, cmd_remove_dir,
		cmd_make_dir, cmd_print_workdir, cmd_list, cmd_system, cmd_noop
	};
	printf("code : %d\nstr : %s\n", cv->cv_code, g_ftp_cmd_str[cv->cv_code]);
	return (command[cv->cv_code](cv));
}

void	listen_clients(int sock)
{
	int		cs;
	int		r;
	char	buff[1024];
	// pid_t	pid;
	t_client_verbs	cv;
	t_server_verbs	sv;

	while(1)
	{
		cs = open_client(sock);
		ft_printf("socket : %d\nclient socket : %d\n", sock, cs);
		// pid = fork();
		while((r = recv(cs, buff, 1023, 0)) > 0)
		{
			buff[r] = '\0';
			ft_printf("received %d bytes : [%s]\n", r, buff);

			sv = ftp_lexer(buff, &cv);
			if (sv.sr_code != _100)
				response_to_client(cs, sv.sr_code, sv.user_info);
			if (sv.sr_state == POS_TMP)
			{
				sv = ftp_parser(&cv);
				response_to_client(cs, sv.sr_code, sv.user_info);
			}

			// ft_printf("received %d bytes on pid %d : [%s]\n", r, pid, buff);
		}
		// ft_printf("Socket with pid %d finished with %s\n", pid, strerror(errno));
		ft_printf("Socket finished with %s\n\trecv : %d\n", strerror(errno), r);
		close(cs);
	}
}

void	getargs(int ac, char** av, struct s_server_args *sa)
{
	char opt;

	if (ac < 2)
		usage(av[0]);
	sa->sa_port = ft_atoi(av[1]);
	sa->sa_root = ft_getcwd();
	while ((opt = (char)getopt(ac, av, "r")) != -1)
	{
		if (opt == 'r')
			sa->sa_root = av[optind];
		else
			usage(av[0]);
	}
}

int		main(int ac, char **av)
{
	t_server_args	sa;
	int 			sock;

	if (ac < 2)
		usage(av[0]);
	getargs(ac, av, &sa);
	if (chdir(sa.sa_root) == -1)
	{
		if (errno == EACCES)
			ft_printf("%s : Permission denied\n", sa.sa_root);
		else if (errno == ENOTDIR)
			ft_printf("%s : Not a directory\n", sa.sa_root);
		else if (errno == ENOENT)
			ft_printf("%s : No such file or directory\n", sa.sa_root);
		else
			ft_printf("chdir failed\n");
		exit(-1);
	}
	(void)get_root();
	sock = create_server(sa.sa_port);
	listen_clients(sock);
	close(sock);

	// TODO : Split .h to remove these lines
	(void)g_user_cmd_str;
	(void)g_ftp_cmd_str;
	(void)g_ftp_code_str;

	return(0);
}
