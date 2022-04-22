/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 23:49:09 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"
#include "ftp_srv_cmd_static.h"

t_bool debug;

void usage(char *str)
{
	printf("Usage: %s <port> [-r <root directory>] [-d <debug>]\n", str);
	;
}

int ftp_create_sock(int port)
{
	int                sock;
	struct protoent   *proto;
	struct sockaddr_in sin;

	proto = getprotobyname("tcp");
	if (!proto)
		exit(EXIT_FAILURE);
	sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
	ft_bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		if (errno == EACCES)
			printf("This address is protected\n");
		else if (errno == EADDRINUSE)
			printf("This address is already in use\n");
		else
			printf("Bind failed");
		exit(EXIT_FAILURE);
	}
	return sock;
}

int ftp_create_channel(int port)
{
	int sock;

	sock = ftp_create_sock(port);
	if (listen(sock, MAX_PENDING_CONNECTIONS) == -1)
	{
		if (errno == ECONNREFUSED)
			printf("The queue is full");
		else
			printf("Listen failed");
		return (-1);
	}
	return (sock);
}

int ftp_accept_connection(int sock)
{
	int                cs;
	struct sockaddr_in csin;
	unsigned int       cslen;

	errno = 0;
	cslen = sizeof(csin);
	if ((cs = accept(sock, (struct sockaddr *)&csin, &cslen)) == -1)
	{
		if (errno == EBADF)
			printf("The file descriptor is invalid\n");
		else if (errno == ECONNABORTED)
			printf("The connection has been aborted\n");
		else
			printf("Accept failed\n\terrno : %d\n\terror : %s\n", errno, strerror(errno));
	}
	return (cs);
}

// TODO : Merge with user_lexer (client.c)
t_server_verbs ftp_lexer(const char *buff, t_client_verbs *cv)
{
	char		 **split;
	int            code_command;
	t_server_verbs sv;

	split = ft_strsplit(buff, ' ');
	cv->cv_verb = ft_strrtrim(split[0]);
	if (debug)
	{
		if (!ft_strcmp(cv->cv_verb, "PASS"))
			printf("\033[0;34mUSER-PI: %s ****\n\033[0m", cv->cv_verb);
		else
			printf("\033[0;34mUSER-PI: %s\033[0m\n", buff);
	}
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
t_server_verbs ftp_parser(t_client_verbs *cv, t_srv_ftp *srv_ftp)
{
	t_server_action command[] = {
		cmd_username, cmd_password, cmd_account, cmd_auth_method, cmd_change_workdir, cmd_change_to_parent_dir, cmd_logout, cmd_port, cmd_passive_mode, cmd_representation_type, cmd_retrieve, cmd_store, cmd_rename_from, cmd_rename_to, cmd_abort, cmd_delete, cmd_remove_dir, cmd_make_dir, cmd_print_workdir, cmd_list, cmd_system, cmd_protection_buffer_size, cmd_protection, cmd_noop};
	return (command[cv->cv_code](cv, srv_ftp));
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

void listen_clients(int sock, SSL_CTX *ctx)
{
	int   r;
	char *buff;
	// pid_t          pid;
	t_client_verbs cv;
	t_server_verbs sv;
	t_srv_ftp     *srv_ftp = ftp_srv_ftp_init();

	srv_ftp->pi.cs = ftp_accept_connection(sock);
	srv_ftp->ctx = &ctx;
	ftp_srv_send_pi(&srv_ftp->pi, _220, "Server available for new user");

	while ((r = get_next_line_wrapper(srv_ftp->pi.cs,
									  srv_ftp->pi.ssl,
									  srv_ftp->pi.ssl_activated,
									  &buff)) >= 0)
	{
		sv = ftp_lexer(buff, &cv);
		if (sv.sr_state == POS_TMP)
			sv = ftp_parser(&cv, srv_ftp);
		ftp_srv_send_pi(&srv_ftp->pi, sv.sr_code, sv.user_info);
	}
	printf("Socket finished with %s\n", strerror(errno));
	if (srv_ftp->pi.ssl)
		shutdown_ssl(srv_ftp->pi.ssl);
	if (srv_ftp->dtp.ssl)
		shutdown_ssl(srv_ftp->dtp.ssl);
	close(srv_ftp->pi.cs);
	if (srv_ftp->dtp.cs)
		close(srv_ftp->dtp.cs);
}

void getargs(int ac, char **av, struct s_server_args *sa)
{
	char opt;

	if (ac < 2)
		usage(av[0]);
	sa->sa_port = ft_atoi(av[1]);
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

void sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		printf("received SIGINT\n");

		// close(sock);
		// close(fd);
	}
}

void ftp_srv_user_prompt()
{
	char *buff;
	while (get_next_line(STDIN_FILENO, &buff) > 0)
	{
		if (!ft_strcasecmp("kill", buff))
			break;
		else
		{
			ft_putendl("Command failed");
			printf("[%s]\n", buff);
		}
	}
}

void ftp_srv_run(int port)
{
	SSL_CTX *ctx;
	int      sock;

	init_openssl();
	ctx = ftp_srv_create_context();
	configure_context(ctx);

	sock = ftp_create_channel(port);
	listen_clients(sock, ctx);
	close(sock);

	SSL_CTX_free(ctx);
	cleanup_openssl();
}

void ftp_print_chdir_error(const char *path)
{
	if (errno == EACCES)
		printf("%s : Permission denied\n", path);
	else if (errno == ENOTDIR)
		printf("%s : Not a directory\n", path);
	else if (errno == ENOENT)
		printf("%s : No such file or directory\n", path);
	else
		printf("chdir failed\n");
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
	int           pid;

	getargs(ac, av, &sa);
	ftp_set_debug(sa.sa_debug);
	ftp_init_root_dir(sa.sa_root);
	pid = fork();
	if (pid != 0)
		ftp_srv_user_prompt();
	else
		ftp_srv_run(sa.sa_port);
	printf("Client disconnected");
	return (EXIT_SUCCESS);
}
