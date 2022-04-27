/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2022/04/25 13:37:15 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include "ftp_srv_cmd_static.h"

t_bool debug;

void usage(char *str)
{
	ft_printf("Usage: %s <port> [-r <root directory>] [-d <debug>] [-i "
			  "<interface>]\n",
			  str);
}

t_srv_res ftp_lexer(const char *buff, t_cli_req *req)
{
	char    **split;
	int       code_command;
	char     *verb;
	t_srv_res response;

	split = ft_strsplit(buff, ' ');
	verb = ft_strrtrim(split[0]);
	if (debug)
	{
		if (!ft_strcmp(verb, cmd_str[PASSWORD]))
			ft_printf("\033[0;34mUSER-PI: %s ****\n\033[0m", verb);
		else
			ft_printf("\033[0;34mUSER-PI: %s\033[0m\n", buff);
	}
	if (!verb || (code_command = ft_arraystr(cmd_str, verb)) == -1)
		return (ftp_build_srv_res(_500, "Unknown command"));
	req->req_arg = ft_strrtrim(split[1]);
	req->req_code = (t_cmd)code_command;
	response = ftp_build_srv_res(_100, "");
	return (response);
}

t_srv_res ftp_parser(t_cli_req *req, t_srv_ftp *srv_ftp)
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
	return (cmd_builtin[req->req_code](req, srv_ftp));
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

void listen_clients(int sock, SSL_CTX *ctx, char *interface)
{
	char      *buff;
	t_cli_req  req;
	t_srv_res  response;
	t_srv_ftp *srv_ftp;

	while (1)
	{
		srv_ftp = ftp_srv_ftp_init();
		srv_ftp->ctx = &ctx;
		srv_ftp->pi.cs = ftp_accept_connection(sock, &srv_ftp->pi.sin);
		srv_ftp->interface = interface;

		ftp_srv_response_pi(&srv_ftp->pi, _220, "Server available for new user");
		while (get_next_line_wrapper(srv_ftp->pi.cs,
									 srv_ftp->pi.ssl,
									 srv_ftp->pi.ssl_activated,
									 &buff,
									 FTP_EOC) > 0)
		{
			response = ftp_lexer(buff, &req);
			if (ftp_get_state_code(response.sr_code) == POS_TMP)
				response = ftp_parser(&req, srv_ftp);
			ftp_srv_response_pi(
				&srv_ftp->pi, response.sr_code, response.user_info);
		}
		ftp_close_connection(&srv_ftp->pi);
		free(srv_ftp);
	}
}

t_bool is_interface_online(char *interface)
{
	int          sockfd;
	struct ifreq ifr;

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		error_print_exit(EXIT_FAILURE, "Create socket failed");
	ft_bzero(&ifr, sizeof(struct ifreq));
	ft_strcpy(ifr.ifr_name, interface);
	return (ioctl(sockfd, SIOCGIFADDR, &ifr) != -1);
}

void getargs(int ac, char **av, t_server_args *sa)
{
	char opt;

	if (ac < 2)
		usage(av[0]);
	sa->sa_port = htons(ft_atoi(av[1]));
	sa->sa_root = ft_getcwd();
	sa->sa_interface = "lo";
	sa->sa_debug = false;
	while ((opt = getopt(ac, av, "rdi")) != -1)
	{
		if (opt == 'r')
			sa->sa_root = av[optind];
		else if (opt == 'd')
			sa->sa_debug = true;
		else if (opt == 'i')
		{
			if (is_interface_online(av[optind]))
				sa->sa_interface = av[optind];
			else
				error_print_exit(EXIT_FAILURE, "Interface is offline");
		}
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

void ftp_srv_run(in_port_t port, char *interface)
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
				listen_clients(sock, ctx, interface);
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
	ftp_srv_run(sa.sa_port, sa.sa_interface);
	return (EXIT_SUCCESS);
}
