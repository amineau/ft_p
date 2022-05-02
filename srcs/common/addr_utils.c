/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addr_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 12:40:41 by amineau           #+#    #+#             */
/*   Updated: 2022/04/28 14:14:15 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"
#include <net/if.h>
#include <sys/ioctl.h>

in_addr_t itoaddr(int *numbers)
{
	return (in_addr_t)((numbers[3] << 24) | (numbers[2] << 16) |
					   (numbers[1] << 8) | numbers[0]);
}

in_port_t itoport(int *numbers)
{
	return (in_port_t)((numbers[0] << 8) | numbers[1]);
}

char *addrtostr(in_addr_t s_addr)
{
	char **addr;
	char  *str;

	addr = (char *[]){
		ft_itoa((s_addr & 0x000000FF)),
		ft_itoa((s_addr & 0x0000FF00) >> 8),
		ft_itoa((s_addr & 0x00FF0000) >> 16),
		ft_itoa((s_addr & 0xFF000000) >> 24),
		NULL,
	};
	str = ft_arraycjoin(addr, ',');
	// ft_arrayfree(addr);
	return str;
}

char *porttostr(in_port_t sin_port)
{
	char **port;
	char  *str;

	port = (char *[]){
		ft_itoa((sin_port & 0x00FF)), ft_itoa((sin_port & 0xFF00) >> 8), NULL};

	str = ft_arraycjoin(port, ',');
	return str;
}

char *get_local_ip(char *interface)
{
	char        *ip;
	int          sockfd;
	struct ifreq req;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		error_print_exit(EXIT_FAILURE, "Create socket failed");
	ft_bzero(&req, sizeof(struct ifreq));
	ft_strcpy(req.ifr_name, interface);
	if (ioctl(sockfd, SIOCGIFADDR, &req) == -1)
		error_print_exit(EXIT_FAILURE, "Get local ip failed");
	struct sockaddr_in *host = (struct sockaddr_in *)&req.ifr_addr;
	ip = addrtostr(host->sin_addr.s_addr);
	close(sockfd);
	return (ip);
}

t_bool is_interface_online(char *interface)
{
	int          sockfd;
	struct ifreq ifr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		error_print_exit(EXIT_FAILURE, "Create socket failed");
	ft_bzero(&ifr, sizeof(struct ifreq));
	ft_strcpy(ifr.ifr_name, interface);
	return (ioctl(sockfd, SIOCGIFADDR, &ifr) != -1);
}