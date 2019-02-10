/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2019/02/10 04:58:28 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void	usage(char *str)
{
	printf("Usage: %s <port> [-r <root directory>] [-d <debug>]\n", str);
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
			printf("This address is protected\n");
		else if(errno == EADDRINUSE)
			printf("This address is already in use\n");
		else
			printf("Bind failed");
		exit(-1);
	}
	if (listen(sock, MAX_PENDING_CONNECTIONS) == -1)
	{
		if (errno == ECONNREFUSED)
			printf("The queue is full");
		else
			printf("Listen failed");
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
t_server_verbs	ftp_parser(t_client_verbs* cv, t_env* env)
{
	t_server_action	command[] = {
		cmd_username, cmd_password, cmd_account, cmd_auth_method, cmd_change_workdir,
		cmd_change_to_parent_dir, cmd_logout, cmd_port, cmd_passive_mode, cmd_representation_type, cmd_retrieve, cmd_store,
		cmd_rename_from, cmd_rename_to, cmd_abort, cmd_delete, cmd_remove_dir,
		cmd_make_dir, cmd_print_workdir, cmd_list, cmd_system, cmd_noop
	};
	printf("code : %d\nstr : %s\n", cv->cv_code, g_ftp_cmd_str[cv->cv_code]);
	return (command[cv->cv_code](cv, env));
}

void	listen_clients(int sock, SSL_CTX *ctx, t_bool debug)
{
	int		r;
	char	buff[BUFF_SIZE];
	// pid_t	pid;
	t_client_verbs	cv;
	t_server_verbs	sv;
	t_env	env;

	while(1)
	{
		env.cs = open_client(sock);
		env.ssl = NULL;
		env.ctx = &ctx;
		env.debug = debug;
		// printf("socket : %d\nclient socket : %d\n", sock, env.cs);
		response_to_client(&env, _220, "Server available for new user");

		// env.ssl = SSL_new(ctx);
        // SSL_set_fd(env.ssl, env.cs);
        
		// printf("SSL_accept : %d\n", SSL_accept(env.ssl));

        // if (SSL_accept(env.ssl) <= 0) {
        //     ERR_print_errors_fp(stderr);
        // }
        // else {
        //     SSL_write(env.ssl, "toot", 4);
        // }
        // printf("ssl first : %p\n", env.ssl);
		// const char * reply = "200 OK\r\n";
		// printf("response SSL_accept : %d\n", SSL_accept(env.ssl));
		// if (SSL_accept(env.ssl) <= 0) {
		// 	printf("plip\n");			
		// 	ERR_print_errors_fp(stderr);
		// 	exit(EXIT_FAILURE);
		// }
		// else
		// {
		// 	printf("plop\n");
		// 	printf("Before write\n");
		// 	SSL_write(env.ssl, reply, ft_strlen(reply));
		// 	printf("After write\n");
		// }
		while((r = received(&env, buff)) > 0)
		{
			printf("while passing\n");
			sv = ftp_lexer(buff, &cv);
			if (sv.sr_state == POS_TMP)
				sv = ftp_parser(&cv, &env);
			response_to_client(&env, sv.sr_code, sv.user_info);
			// printf("received %d bytes on pid %d : [%s]\n", r, pid, buff);
		}
		// printf("Socket with pid %d finished with %s\n", pid, strerror(errno));
		printf("Socket finished with %s\n", strerror(errno));
        if (env.ssl)
			SSL_free(env.ssl);
		close(env.cs);
	}
}

void	getargs(int ac, char** av, struct s_server_args *sa)
{
	char opt;

	if (ac < 2)
		usage(av[0]);
	sa->sa_port = ft_atoi(av[1]);
	sa->sa_root = ft_getcwd();
	sa->debug = false;
	while ((opt = (char)getopt(ac, av, "rd")) != -1)
	{
		if (opt == 'r')
			sa->sa_root = av[optind];
		else if (opt == 'd')
			sa->debug = true;
		else
			usage(av[0]);
	}
	if (chdir(sa->sa_root) == -1)
	{
		if (errno == EACCES)
			printf("%s : Permission denied\n", sa->sa_root);
		else if (errno == ENOTDIR)
			printf("%s : Not a directory\n", sa->sa_root);
		else if (errno == ENOENT)
			printf("%s : No such file or directory\n", sa->sa_root);
		else
			printf("chdir failed\n");
		exit(-1);
	}
}

void	sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		printf("received SIGINT\n");

		// close(sock);
		// close(fd);
	}
}

int		main(int ac, char **av)
{
	t_server_args	sa;
	int 			sock;
	SSL_CTX *ctx;
	int pid;

	getargs(ac, av, &sa);

	pid = fork();
	if (pid != 0)
	{
		char buff[BUFF_SIZE];
		int r;
		while((r = read(STDIN_FILENO, buff, BUFF_SIZE)))
		{
			buff[r - 1] = '\0';
			if (!ft_strcasecmp("kill", buff))
			{
				ft_putendl("Server exited");
				exit(-1);
			}
			else
			{
				ft_putendl("Command failed");
				printf("[%s]\n", buff);
			}
		}
	}
	else
	{
		init_openssl();
		ctx = create_context();
		configure_context(ctx);
		
		(void)get_root();
		sock = create_server(sa.sa_port);
		listen_clients(sock, ctx, sa.debug);
		close(sock);
		SSL_CTX_free(ctx);
		cleanup_openssl();
		// TODO : Split .h to remove these lines
		(void)g_user_cmd_str;
		(void)g_ftp_cmd_str;
		(void)g_ftp_code_str;
	}
	return(0);
}
