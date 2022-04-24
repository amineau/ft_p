/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 02:42:16 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include "ftp_srv_cmd_static.h"

t_bool debug;

void usage(char *str)
{
	ft_printf("Usage: %s <port> [-r <root directory>] [-d <debug>]\n", str);
	;
}

t_server_verbs ftp_lexer(const char *buff, t_client_verbs *cv)
{
	char		 **split;
	int            code_command;
	t_server_verbs sv;

	split = ft_strsplit(buff, ' ');
	cv->cv_verb = ft_strrtrim(split[0]);
	if (debug)
	{
		if (!ft_strcmp(cv->cv_verb, cmd_str[PASSWORD]))
			ft_printf("\033[0;34mUSER-PI: %s ****\n\033[0m", cv->cv_verb);
		else
			ft_printf("\033[0;34mUSER-PI: %s\033[0m\n", buff);
	}
	if (!cv->cv_verb || (code_command = ft_arraystr(cmd_str, cv->cv_verb)) == -1)
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

t_server_verbs ftp_parser(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_action cmd_builtin[] = {
		[USERNAME] = cmd_username,
		[PASSWORD] = cmd_password,
		[ACCOUNT] = cmd_account,
		[AUTH_METHOD] = cmd_auth_method,
		[CHANGE_WORKDIR] = cmd_change_workdir,
		[CHANGE_TO_PARENT_DIR] = cmd_change_to_parent_dir,
		[LOGOUT] = cmd_logout,
		[PORT] = cmd_port,
		[PASSIVE_MODE] = cmd_passive_mode,
		[REPRESENTATION_TYPE] = cmd_representation_type,
		[RETRIEVE] = cmd_retrieve,
		[STORE] = cmd_store,
		[RENAME_FROM] = cmd_rename_from,
		[RENAME_TO] = cmd_rename_to,
		[ABORT] = cmd_abort,
		[DELETE] = cmd_delete,
		[REMOVE_DIR] = cmd_remove_dir,
		[MAKE_DIR] = cmd_make_dir,
		[PRINT_WORKDIR] = cmd_print_workdir,
		[LIST] = cmd_list,
		[SYSTEM] = cmd_system,
		[PROTECTION_BUFFER_SIZE] = cmd_protection_buffer_size,
		[PROTECTION] = cmd_protection,
		[NOOP] = cmd_noop,
		NULL,
	};
	return (cmd_builtin[cv->cv_code](cv, srv_ftp));
}

t_srv_ftp *ftp_srv_ftp_init()
{
	t_srv_ftp *srv_ftp;

	srv_ftp = (t_srv_ftp *)malloc(sizeof(t_srv_ftp));
	srv_ftp->ctx = NULL;
	srv_ftp->pi.cs = 0;
	srv_ftp->pi.ssl = NULL;
	srv_ftp->dtp.cs = 0;
	srv_ftp->dtp.ssl = NULL;
	srv_ftp->pi.ssl_activated = false;
	srv_ftp->dtp.ssl_activated = false;
	return (srv_ftp);
}

void ftp_connection_on_close(t_srv_ftp *srv_ftp)
{
	char str[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &(srv_ftp->pi.sin.sin_addr), str, INET_ADDRSTRLEN);
	ft_printf("Disconnect from %s:%d\n", str, ntohs(srv_ftp->pi.sin.sin_port));
	if (srv_ftp->pi.ssl)
		shutdown_ssl(srv_ftp->pi.ssl);
	if (srv_ftp->dtp.ssl)
		shutdown_ssl(srv_ftp->dtp.ssl);
	close(srv_ftp->pi.cs);
	if (srv_ftp->dtp.cs)
		close(srv_ftp->dtp.cs);
	free(srv_ftp);
}

void listen_clients(int sock, SSL_CTX *ctx)
{
	char          *buff;
	t_client_verbs cv;
	t_server_verbs sv;
	t_srv_ftp     *srv_ftp;

	while (1)
	{
		srv_ftp = ftp_srv_ftp_init();
		srv_ftp->ctx = &ctx;
		srv_ftp->pi.cs = ftp_accept_connection(sock, &srv_ftp->pi.sin);

		ftp_srv_send_pi(&srv_ftp->pi, _220, "Server available for new user");
		while (get_next_line_wrapper(srv_ftp->pi.cs,
									 srv_ftp->pi.ssl,
									 srv_ftp->pi.ssl_activated,
									 &buff,
									 FTP_EOC) > 0)
		{
			sv = ftp_lexer(buff, &cv);
			if (sv.sr_state == POS_TMP)
				sv = ftp_parser(&cv, srv_ftp);
			ftp_srv_send_pi(&srv_ftp->pi, sv.sr_code, sv.user_info);
		}
		ftp_connection_on_close(srv_ftp);
	}
}

void getargs(int ac, char **av, t_server_args *sa)
{
	char opt;

	if (ac < 2)
		usage(av[0]);
	sa->sa_port = htons(ft_atoi(av[1]));
	sa->sa_root = ft_getcwd();
	sa->sa_debug = false;
	while ((opt = (char)getopt(ac, av, "rd")) != -1)
	{
		if (opt == 'r')
			sa->sa_root = av[optind];
		else if (opt == 'd')
			sa->sa_debug = true;
		else
			usage(av[0]);
	}
}

void ftp_srv_user_prompt()
{
	char *buff;
	while (get_next_line(STDIN_FILENO, &buff) > 0)
	{
		if (!ft_strcasecmp("quit", buff))
			exit(EXIT_SUCCESS);
		else
		{
			ft_putendl("Command failed");
			ft_printf("[%s]\n", buff);
		}
	}
}

void ftp_srv_run(in_port_t port)
{
	int                i;
	SSL_CTX           *ctx;
	int                sock;
	struct sockaddr_in sin;
	int                pid;
	pid_t              pids[MAX_CLIENT_CONNECTION];
	int                status;

	ctx = ftp_srv_create_context();
	configure_context(ctx);

	i = 0;
	if ((pid = fork()) != 0)
	{
		init_openssl();

		sin = ftp_get_socket_address(stoaddr(htonl(INADDR_ANY)), port);
		sock = ftp_create_socket();
		ftp_bind_socket(sock, &sin);
		ftp_listen_connection(sock);
		while (i < MAX_CLIENT_CONNECTION)
		{
			pids[i] = fork();
			if (pids[i] == 0)
				listen_clients(sock, ctx);
			i++;
		}
		ft_printf("Server started on port %d\n", ntohs(port));
		close(sock);
		wait4(pid, &status, 0, 0);
		i = 0;
		while (i < MAX_CLIENT_CONNECTION)
			kill(pids[i++], SIGTERM);
		SSL_CTX_free(ctx);
		cleanup_openssl();
		ft_printf("Server stopped\n");
		exit(0);
	}
	else
		ftp_srv_user_prompt();
}

void ftp_print_chdir_error(const char *path)
{
	if (errno == EACCES)
		ft_printf("%s : Permission denied\n", path);
	else if (errno == ENOTDIR)
		ft_printf("%s : Not a directory\n", path);
	else if (errno == ENOENT)
		ft_printf("%s : No such file or directory\n", path);
	else
		ft_printf("chdir failed\n");
}

void ftp_init_root_dir(const char *rootpath)
{
	if (chdir(rootpath) == -1)
	{
		ftp_print_chdir_error(rootpath);
		exit(EXIT_FAILURE);
	}
	init_root_static();
}

void ftp_set_debug(t_bool sa_debug)
{
	debug = sa_debug;
}

int main(int ac, char **av)
{
	t_server_args sa;

	getargs(ac, av, &sa);
	ftp_set_debug(sa.sa_debug);
	ftp_init_root_dir(sa.sa_root);
	ftp_srv_run(sa.sa_port);
	return (EXIT_SUCCESS);
}
