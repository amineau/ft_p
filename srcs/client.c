/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 19:06:20 by amineau           #+#    #+#             */
/*   Updated: 2018/08/17 01:50:12 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void	usage(char *str)
{
	ft_printf("Usage: %s <host> <port> [-u <user name> [-p <passwd>]] -\n", str);
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
			ft_printf("%s is unknown\n", name);
		else if (h_errno == NO_DATA)
			ft_printf("%s does not have an IP address\n", name);
		else if (h_errno == NO_RECOVERY)
			ft_printf("Server error\n");
		else if (h_errno == TRY_AGAIN)
			ft_printf("A temporary error occurred on an authoritative name server.  Try again later.\n");
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
			ft_printf("Local address is already in use\n");
		else if (errno == ECONNREFUSED)
			ft_printf("Remote address not listening\n");
		else
			ft_printf("Connect failed\n");
		exit(-1);
	}
	ft_printf("Client connected\n");
	return(sock);
}

void	getargs(int ac, char** av, struct s_client_args *ca)
{
	char opt;

	if (ac < 3)
		usage(av[0]);
	ca->ca_host = htoaddr(av[1]);
	ca->ca_port = ft_atoi(av[2]);
	while ((opt = (char)getopt(ac, av, "upd")) != -1)
	{
		if (opt == 'u')
			ca->ca_user = av[optind];
		else if (opt == 'p')
			ca->ca_pass = av[optind];
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
		ft_printf("Unkwown command : [%s]\nType help for more information\n");
		return (-1);
	}
	cv->cv_verb = split[0];
	cv->cv_arg = split[1];
	cv->cv_code = code_command;
	return (0);
}

void	put_req_arg(char *cmd)
{
	ft_printf("Argument is required for this command [%s]\n", cmd);
}

void	put_no_req_arg(char *cmd)
{
	ft_printf("This command [%s] is used without arguments\n", cmd);
}

char	*list(t_client_verbs *cv)
{
	return (ft_strcjoin(LIST, cv->cv_arg, ' '));
}

char	*change_workdir(t_client_verbs *cv)
{
	char	*cmd;

	if (!cv->cv_arg)
		cmd = ft_strdup(CHANGE_TO_PARENT_DIR);
	else
		cmd = ft_strcjoin(CHANGE_WORKDIR, cv->cv_arg, ' ');
	return (cmd);
}

char	*get_file(t_client_verbs *cv)
{
	if (!cv->cv_arg)
	{
		put_req_arg(cv->cv_verb);
		return (NULL);
	}
	return (ft_strcjoin(RETRIEVE, cv->cv_arg, ' '));
}

char	*put_file(t_client_verbs *cv)
{
	if (!cv->cv_arg)
	{
		put_req_arg(cv->cv_verb);
		return (NULL);
	}
	return (ft_strcjoin(STORE, cv->cv_arg, ' '));
}

char	*print_workdir(t_client_verbs *cv)
{
	if (cv->cv_arg)
	{
		put_no_req_arg(cv->cv_verb);
		return (NULL);
	}
	return (ft_strdup(PRINT_WORKDIR));
}

char	*logout(t_client_verbs *cv)
{
	if (cv->cv_arg)
	{
		put_no_req_arg(cv->cv_verb);
		return (NULL);
	}
	return (ft_strdup(LOGOUT));
}

char	*help(t_client_verbs *cv)
{
	(void)cv;
	ft_printf("TODO : help command\n");
	return (NULL);
}

// TODO : Merge with ftp_parser (server.c)
char*	user_parser(t_client_verbs* cv)
{
	t_client_action	command[] = {
		list, change_workdir, get_file, put_file, print_workdir, logout, help
	};

	return (command[cv->cv_code](cv));
}

int		send_to_remote(int sock, char* cmd)
{
	int	nb_writed_char;

	nb_writed_char = ft_printf_fd(sock, "%s%s", cmd, FTP_EOC);
	return nb_writed_char;
}

void	print_to_client(char *cmd)
{
	ft_printf("%s ---->%c", cmd, LF);
}

int		main(int ac, char **av)
{
	int		sock;
	char	*buff;
	int		gnllen;
	t_client_args	ca;
	t_client_verbs	cv;
	char*	cmd;

	getargs(ac, av, &ca);
	sock = create_client(ca.ca_host , ca.ca_port);
	while((gnllen = get_next_line(STDIN_FILENO, &buff)) > 0)
	{
		if (user_lexer(buff, &cv) == -1)
			continue;
		if ((cmd = user_parser(&cv)))
		{
			send_to_remote(sock, cmd);
			print_to_client(cmd);
			free(cmd);
		}
	}
	ft_printf("Client disconnected\n");
	close(sock);

	// TODO : Split .h to remove these lines
	(void)g_user_cmd_str;
	(void)g_ftp_cmd_str;
	(void)g_ftp_code_str;

	return (0);
}
