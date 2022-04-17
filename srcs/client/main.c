/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2022/04/18 01:42:51 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void	usage(char *str)
{
	printf("Usage: %s <host> <port> [-u <user name> [-p <passwd>]] [-d <debug>] -\n", str);
	exit(-1);
}

struct in_addr	htoaddr(char *name)
{
	struct hostent	*host;
	struct in_addr	addr;

	host = gethostbyname(name);
	if (!host)
	{
		if (h_errno == HOST_NOT_FOUND)
			printf("%s is unknown\n", name);
		else if (h_errno == NO_DATA)
			printf("%s does not have an IP address\n", name);
		else if (h_errno == NO_RECOVERY)
			printf("Server error\n");
		else if (h_errno == TRY_AGAIN)
			printf("A temporary error occurred on an authoritative name server.  Try again later.\n");
		exit(-1);
	}
	ft_memcpy(&addr.s_addr, host->h_addr, host->h_length);
	return(addr);
}

int		create_client(struct in_addr host, int port)
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
	sin.sin_addr = host;
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		if (errno == EADDRINUSE)
			printf("Local address is already in use\n");
		else if (errno == ECONNREFUSED)
			printf("Remote address not listening\n");
		else
			printf("Connect failed\n");
		exit(-1);
	}
	printf("Client connected\n");
	return(sock);
}

void	getargs(int ac, char** av, struct s_client_args *ca)
{
	char opt;

	if (ac < 3)
		usage(av[0]);
	ca->ca_host = htoaddr(av[1]);
	ca->ca_port = ft_atoi(av[2]);
	ca->debug = false;
	while ((opt = (char)getopt(ac, av, "upd")) != -1)
	{
		if (opt == 'u')
			ca->ca_user = av[optind];
		else if (opt == 'p')
			ca->ca_pass = av[optind];
		else if (opt == 'd')
			ca->debug = true;
		else
			usage(av[0]);
	}
}

// TODO : Merge with ftp_lexer (server.c)
int	user_lexer(const char *buff, t_client_verbs* cv)
{
	char	**split;
	int		code_command;

	split = ft_strsplit(buff, ' ');
	if (!split[0])
		return (-1);
	else if ((code_command = ft_arraystr(g_user_cmd_str, split[0])) == -1)
	{
		printf("Unkwown command : [%s]\nType help for more information\n", split[0]);
		return (-1);
	}
	cv->cv_verb = split[0];
	cv->cv_arg = split[1];
	cv->cv_code = code_command;
	return (0);
}

void	put_req_arg(char *cmd)
{
	printf("Argument is required for this command [%s]\n", cmd);
}

void	put_no_req_arg(char *cmd)
{
	printf("This command [%s] is used without arguments\n", cmd);
}

char	*list(t_client_verbs *cv, int sock)
{
	return (ft_strcjoin(LIST, cv->cv_arg, ' '));
}

char	*change_workdir(t_client_verbs *cv, int sock)
{
	char	*cmd;

	if (!cv->cv_arg)
		cmd = ft_strdup(CHANGE_TO_PARENT_DIR);
	else
		cmd = ft_strcjoin(CHANGE_WORKDIR, cv->cv_arg, ' ');
	return (cmd);
}

char	*get_file(t_client_verbs *cv, int sock)
{
	if (!cv->cv_arg)
	{
		put_req_arg(cv->cv_verb);
		return (NULL);
	}
	return (ft_strcjoin(RETRIEVE, cv->cv_arg, ' '));
}

char	*put_file(t_client_verbs *cv, int sock)
{
	if (!cv->cv_arg)
	{
		put_req_arg(cv->cv_verb);
		return (NULL);
	}
	return (ft_strcjoin(STORE, cv->cv_arg, ' '));
}

char	*print_workdir(t_client_verbs *cv, int sock)
{
	if (cv->cv_arg)
	{
		put_no_req_arg(cv->cv_verb);
		return (NULL);
	}
	return (ft_strdup(PRINT_WORKDIR));
}

char	*logout(t_client_verbs *cv, int sock)
{
	if (cv->cv_arg)
	{
		put_no_req_arg(cv->cv_verb);
		return (NULL);
	}
	return (ft_strdup(LOGOUT));
}

char	*help(t_client_verbs *cv, int sock)
{
	(void)cv;
	printf("TODO : help command\n");
	return (NULL);
}

// TODO : Merge with ftp_parser (server.c)
char*	user_parser(t_client_verbs* cv, int sock)
{
	t_client_action	command[] = {
		list, change_workdir, get_file, put_file, print_workdir, logout, help
	};

	return (command[cv->cv_code](cv, sock));
}

int		ftp_client_send_pi(t_srv_ftp *srv_ftp, char* cmd)
{
	int		ret;
	char 	*str;

	str = ft_strjoin(cmd, FTP_EOC);
	if (srv_ftp->debug == true)
		ft_printf("USER-PI: %s", str);
	if (srv_ftp->ssl_activated == true)
		ret = SSL_write(srv_ftp->pi.ssl, str, ft_strlen(str));
	else
		ret = write(srv_ftp->pi.cs, str, ft_strlen(str));
	free(str);
	return (ret);
}

char	listen_server(t_srv_ftp *srv_ftp)
{
	int		r;
	char	buff[BUFF_SIZE];

	if ((r = received(srv_ftp, buff)) > 0)
	{
		if (srv_ftp->debug)
			printf("SERVER-PI: %s", buff);
		return(buff[0]);
	}
	exit(EXIT_FAILURE);
}

int		connection_protocol(t_srv_ftp *srv_ftp, t_client_args	*ca)
{
	char	code_response;
	int ret;

	while((code_response = listen_server(srv_ftp)) != '2')
		;		
	ftp_client_send_pi(srv_ftp, "AUTH TLS");
	srv_ftp->pi.ssl = SSL_new(*srv_ftp->ctx);
	SSL_set_fd(srv_ftp->pi.ssl, srv_ftp->pi.cs);
	while((code_response = listen_server(srv_ftp)) != '2')
		;
	if ((ret = SSL_connect(srv_ftp->pi.ssl)) <= 0){
		printf("SSL_connect failed\n");
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
	// printf("Connected with %s encryption\n", SSL_get_cipher(srv_ftp->pi.ssl));
	// ShowCerts(srv_ftp->pi.ssl);
	srv_ftp->ssl_activated = true;
	ftp_client_send_pi(srv_ftp, ft_strjoin("USER ", ca->ca_user));
	while((code_response = listen_server(srv_ftp)) != '3')
		;
	ftp_client_send_pi(srv_ftp, ft_strjoin("PASS ", ca->ca_pass));
	while((code_response = listen_server(srv_ftp)) != '2')
		;
	ftp_client_send_pi(srv_ftp, ft_strjoin("PASS ", ca->ca_pass));
}

int		main(int ac, char **av)
{
	char	*buff;
	int		gnllen;
	t_client_args	ca;
	t_client_verbs	cv;
	char*	cmd;
	t_srv_ftp	srv_ftp;
	SSL_CTX * ctx;

	getargs(ac, av, &ca);
	
	init_openssl();
	ctx = ftp_client_create_context();
	srv_ftp.ctx = &ctx;
	srv_ftp.pi.cs = create_client(ca.ca_host , ca.ca_port);
	srv_ftp.pi.ssl = false;
	srv_ftp.ssl_activated = false;
	srv_ftp.debug = ca.debug;

	connection_protocol(&srv_ftp, &ca);
	while((gnllen = get_next_line(STDIN_FILENO, &buff)) > 0)
	{
		if (user_lexer(buff, &cv) == -1)
			continue;
		if ((cmd = user_parser(&cv, srv_ftp.pi.cs)))
		{
			ftp_client_send_pi(&srv_ftp, cmd);
			free(cmd);
		}
	}
	SSL_free(srv_ftp.pi.ssl);

	ft_printf("Client disconnected\n");
	close(srv_ftp.pi.cs);
	SSL_CTX_free(*srv_ftp.ctx);
	cleanup_openssl();

	// TODO : Split .h to remove these lines
	(void)g_user_cmd_str;
	(void)g_ftp_cmd_str;
	(void)g_ftp_code_str;

	return (0);
}
