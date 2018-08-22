/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2018/08/20 00:01:31 by amineau          ###   ########.fr       */
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
		cmd_change_to_parent_dir, cmd_logout, cmd_representation_type, cmd_retrieve, cmd_store,
		cmd_rename_from, cmd_rename_to, cmd_abort, cmd_delete, cmd_remove_dir,
		cmd_make_dir, cmd_print_workdir, cmd_list, cmd_system, cmd_noop
	};
	printf("code : %d\nstr : %s\n", cv->cv_code, g_ftp_cmd_str[cv->cv_code]);
	return (command[cv->cv_code](cv, env));
}

t_bool	is_valid_tls(char *str, size_t len)
{
	return ((!ft_strcmp(&str[len - 2], CRLF)));
}

char*	_received(int fd, SSL *ssl)
{
	char	buff[BUFF_SIZE];
	char*	ptr;
	char*	ret;
	int		r;

	if (ssl_activated(false) == true)
		r = SSL_read(ssl, buff, BUFF_SIZE - 1);
	else
		r = read(fd, buff, BUFF_SIZE - 1);
	ERR_print_errors_fp(stderr);
	// printf("********* r = %d\n********* errno = %s\n",r, strerror(errno));
	if (r <=0)
		return (NULL);
	buff[r] = '\0';

	if (r != 0 && !is_valid_tls(buff, r))
	{
		ret = _received(fd, ssl);
		ft_strncat(buff, ret, BUFF_SIZE);
		ft_strdel(&ret);
	}
	ptr = (char*)malloc(sizeof(char) * ft_strlen(buff));
	ft_strcpy(ptr, buff);
	return (ptr);
}

int		received(int fd, SSL *ssl, char* buff)
{
	char*	ret;

	buff[0] = '\0';
	ret = _received(fd, ssl);
	ft_strcpy(buff, ret);
	ft_strdel(&ret);
	return ft_strlen(buff);
}

void	listen_clients(int sock, SSL_CTX *ctx)
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
		ft_printf("socket : %d\nclient socket : %d\n", sock, env.cs);
		// pid = fork();
		env.ssl = SSL_new(ctx);
        SSL_set_fd(env.ssl, env.cs);

        printf("ssl first : %p\n", env.ssl);
		const char * reply = "200 OK\r\n";

		if (SSL_accept(env.ssl) <= 0) {
			ERR_print_errors_fp(stderr);
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("Before write\n");
			SSL_write(env.ssl, reply, ft_strlen(reply));
			printf("After write\n");
		}
		response_to_client(env.cs, _220, "Server available for new user");
		while((r = received(env.cs, env.ssl, buff)) > 0)
		{
			printf("while passing\n");
			sv = ftp_lexer(buff, &cv);
			if (sv.sr_state == POS_TMP)
				sv = ftp_parser(&cv, &env);
			response_to_client(env.cs, sv.sr_code, sv.user_info);
			// ft_printf("received %d bytes on pid %d : [%s]\n", r, pid, buff);
		}
		// ft_printf("Socket with pid %d finished with %s\n", pid, strerror(errno));
		ft_printf("Socket finished with %s\n", strerror(errno));
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
	while ((opt = (char)getopt(ac, av, "r")) != -1)
	{
		if (opt == 'r')
			sa->sa_root = av[optind];
		else
			usage(av[0]);
	}
	if (chdir(sa->sa_root) == -1)
	{
		if (errno == EACCES)
			ft_printf("%s : Permission denied\n", sa->sa_root);
		else if (errno == ENOTDIR)
			ft_printf("%s : Not a directory\n", sa->sa_root);
		else if (errno == ENOENT)
			ft_printf("%s : No such file or directory\n", sa->sa_root);
		else
			ft_printf("chdir failed\n");
		exit(-1);
	}
}

int		main(int ac, char **av)
{
	t_server_args	sa;
	int 			sock;
	SSL_CTX *ctx;

	getargs(ac, av, &sa);

    init_openssl();
    ctx = create_context();

    configure_context(ctx);
	(void)get_root();
	sock = create_server(sa.sa_port);
	
	listen_clients(sock, ctx);
	
	close(sock);
	SSL_CTX_free(ctx);
    cleanup_openssl();
	// TODO : Split .h to remove these lines
	(void)g_user_cmd_str;
	(void)g_ftp_cmd_str;
	(void)g_ftp_code_str;

	return(0);
}
